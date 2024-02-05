#include "COrthoLevel.h"

COrthoLevel::COrthoLevel()
{
	memset(this, 0x00, sizeof(COrthoLevel));
}

COrthoLevel::COrthoLevel(CGlobalObjects* globalObjects, CTexture* texture, const char* filename, CVertex2* cellSize, CVertex2* scaleSize)
{
	memset(this, 0x00, sizeof(COrthoLevel));

	m_globalObjects = globalObjects;

	m_dungeonTileset = texture;

	m_name = new CString(filename);


	// Need to make individual command allocators and command lists to accomodate threading
	for (UINT i = 0; i < CVideoDevice::BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i] = m_globalObjects->m_videoDevice->CreateCommandAllocator();
	}

	m_commandList = m_globalObjects->m_videoDevice->CreateCommandList(m_commandAllocators[0]);

	m_commandList->SetName(MBSToWide(m_name->GetText()));


	m_wvp = new CFloat4x4Buffer(m_globalObjects, m_commandList);

	m_floats = new CFloatBuffer(m_globalObjects, m_commandList);

	m_ints = new CIntBuffer(m_globalObjects, m_commandList);


	float xScale = (m_globalObjects->m_videoDevice->m_swapChainViewport->m_viewport.Width > 0) ? 2.0f / m_globalObjects->m_videoDevice->m_swapChainViewport->m_viewport.Width : 0.0f;
	float yScale = (m_globalObjects->m_videoDevice->m_swapChainViewport->m_viewport.Height > 0) ? 2.0f / m_globalObjects->m_videoDevice->m_swapChainViewport->m_viewport.Height : 0.0f;

	XMMATRIX wvpMat
	(
		xScale, 0, 0, 0,
		0, -yScale, 0, 0,
		0, 0, 1, 0,
		-1, 1, 0, 1
	);

	XMStoreFloat4x4(&m_wvp->m_values[0], wvpMat);


	m_shaderHeap = new CShaderHeap(m_globalObjects, m_commandList, 4);

	// These resources must be mapped to the pixel shader buffers
	m_shaderHeap->SlotResource(0, m_wvp);

	m_shaderHeap->SlotResource(1, m_floats);

	m_shaderHeap->SlotResource(2, m_ints);

	m_shaderHeap->SlotResource(3, m_dungeonTileset);


	m_range = new CRange();

	// the number three represents the number of constant buffers
	m_range->SetRange(0, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 3);

	// the number one represents the number of texture buffers
	m_range->SetRange(1, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1);

	// the root signature defines the number of samplers
	m_rootSignature = new CRootSignature(m_globalObjects, 2, m_range);


	m_vertexShader = m_globalObjects->m_shaderMgr->MakeShader("vertexImage.hlsl", "VSMain", "vs_5_0");
	m_pixelShader = m_globalObjects->m_shaderMgr->MakeShader("pixelImage.hlsl", "PSMain", "ps_5_0");


	m_pipelineState = new CPipelineState(m_globalObjects, CVertex::VT_VERTEXT);

	m_pipelineState->SetDesc(false, true, false, D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, m_vertexShader, m_pixelShader, nullptr, nullptr, nullptr, m_rootSignature->m_signature);


	FILE* file = {};

	errno_t err = fopen_s(&file, filename, "r");

	if (err)
	{
		m_globalObjects->m_errorLog->WriteError("COrthoLevel::COrthoLevel::Ortholevel not found:%s\n", filename);

		return;
	}

	char sizeBuffer[4] = {};

	char readBuffer;

	size_t bytesRead = fread_s(&sizeBuffer, 3 * sizeof(char), sizeof(char), 3, file);

	m_width = atoi(sizeBuffer);

	bytesRead = fread_s(&sizeBuffer, 3 * sizeof(char), sizeof(char), 3, file);

	m_height = atoi(sizeBuffer);

	bytesRead = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, file);



	m_tiles = new CHeapArray(sizeof(COrthoTile), true, true, 2, m_width, m_height);

	m_masterTileList = new CLinkList<COrthoTile>();

	m_cellSize = *cellSize;

	m_scaleSize.p.x = scaleSize->p.x * (UINT)((float)m_globalObjects->m_window->m_width / (float)m_globalObjects->m_window->m_height);
	m_scaleSize.p.y = scaleSize->p.y * (UINT)((float)m_globalObjects->m_window->m_width / (float)m_globalObjects->m_window->m_height);

	m_position.p.x = (float)((m_globalObjects->m_window->m_width) - (m_scaleSize.p.x * m_width)) / 2;
	m_position.p.y = (float)((m_globalObjects->m_window->m_height) - (m_scaleSize.p.y * m_height)) / 2;


	CVertex2 sheetSize((float)m_dungeonTileset->m_width, (float)m_dungeonTileset->m_height);

	UINT widthCount = 0;
	UINT heightCount = 0;

	UINT redStartCount = 0;
	UINT blueStartCount = 0;

	bytesRead = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, file);

	while (!feof(file))
	{
		switch (readBuffer)
		{
			case ' ':
			{
				CVertex2 cellPosition(17.0f, 16.0f);

				m_tile = new COrthoTile(m_globalObjects, m_commandList, COrthoTile::OT_NONE, &sheetSize, cellSize, &cellPosition, scaleSize, &m_position);

				break;
			}

			case 'A':
			{
				CVertex2 cellPosition(17.0f, 17.0f);

				m_tile = new COrthoTile(m_globalObjects, m_commandList, COrthoTile::OT_CLIP, &sheetSize, cellSize, &cellPosition, scaleSize, &m_position);

				break;
			}

			case 'R':
			{
				CVertex2 cellPosition(4.0f, 16.0f);

				m_tile = new COrthoTile(m_globalObjects, m_commandList, COrthoTile::OT_REDSTART, &sheetSize, cellSize, &cellPosition, scaleSize, &m_position);

				break;
			}

			case 'B':
			{
				CVertex2 cellPosition(27.0f, 16.0f);

				m_tile = new COrthoTile(m_globalObjects, m_commandList, COrthoTile::OT_BLUESTART, &sheetSize, cellSize, &cellPosition, scaleSize, &m_position);

				break;
			}

			case 'L':
			{
				CVertex2 cellPosition(39.0f, 17.0f);

				m_tile = new COrthoTile(m_globalObjects, m_commandList, COrthoTile::OT_LOCK, &sheetSize, cellSize, &cellPosition, scaleSize, &m_position);

				break;
			}

			case 'K':
			{
				CVertex2 cellPosition(29.0f, 16.0f);

				m_tile = new COrthoTile(m_globalObjects, m_commandList, COrthoTile::OT_KEY, &sheetSize, cellSize, &cellPosition, scaleSize, &m_position);

				break;
			}

			case '\n':
			{
				m_tile = nullptr;

				break;
			}

			default:
			{
				m_tile = nullptr;

				break;
			}
		}

		if (m_tile)
		{
			COrthoTile* tile = (COrthoTile*)m_tiles->GetElement(2, widthCount, heightCount);

			// Copying the tile into the heap array so tiles can be referenced in a two dimensional array
			memcpy(tile, m_tile, sizeof(COrthoTile));

			// Maintaining a master list for cleanup
			m_masterTileList->Add(m_tile, 0);

			switch (tile->m_type)
			{
				case COrthoTile::OT_LOCK:
				{
					m_lock = tile;

					break;
				}

				case COrthoTile::OT_KEY:
				{
					m_key = tile;

					break;
				}

				case COrthoTile::OT_REDSTART:
				{
					m_redStart[redStartCount] = tile;
					redStartCount++;
					m_redCount = redStartCount;

					break;
				}

				case COrthoTile::OT_BLUESTART:
				{
					m_blueStart[blueStartCount] = tile;
					blueStartCount++;
					m_blueCount = blueStartCount;

					break;
				}
			}

			widthCount++;

			if (widthCount == m_width)
			{
				m_position.p.x = (float)((m_globalObjects->m_window->m_width) - (scaleSize->p.x * m_width)) / 2;

				m_position.p.y += scaleSize->p.y;

				widthCount = 0;

				heightCount++;
			}
			else
			{
				m_position.p.x += scaleSize->p.x;
			}
		}

		bytesRead = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, file);
	}

	fclose(file);
}

COrthoLevel::~COrthoLevel()
{
	m_globalObjects->m_videoDevice->WaitForGPU();

	delete m_pipelineState;
	delete m_rootSignature;
	delete m_range;
	delete m_shaderHeap;
	delete m_masterTileList;
	delete m_tiles;
	delete m_ints;
	delete m_floats;
	delete m_wvp;

	m_commandList.Reset();

	for (int i = 0; i < CVideoDevice::BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i].Reset();
	}

	delete m_name;
}

void COrthoLevel::Draw()
{
	m_commandAllocators[m_globalObjects->m_videoDevice->m_backbufferIndex]->Reset();

	m_commandList->Reset(m_commandAllocators[m_globalObjects->m_videoDevice->m_backbufferIndex].Get(), nullptr);


	m_commandList->RSSetViewports(1, &m_globalObjects->m_videoDevice->m_swapChainViewport->m_viewport);

	m_commandList->RSSetScissorRects(1, &m_globalObjects->m_videoDevice->m_swapChainViewport->m_scissorRect);

	m_commandList->OMSetRenderTargets(1, &m_globalObjects->m_videoDevice->m_swapChainRenderTargets[m_globalObjects->m_videoDevice->m_backbufferIndex]->m_handle, false, nullptr);


	m_commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_commandList->SetGraphicsRootSignature(m_rootSignature->m_signature.Get());

	m_commandList->SetPipelineState(m_pipelineState->m_pipelineState.Get());


	m_wvp->UpdateConstantBuffer();

	m_floats->UpdateConstantBuffer();

	m_ints->UpdateConstantBuffer();


	m_commandList->SetDescriptorHeaps(1, m_shaderHeap->m_heap.GetAddressOf());

	m_commandList->SetGraphicsRootDescriptorTable(0, m_shaderHeap->m_heap->GetGPUDescriptorHandleForHeapStart());


	for (UINT h = 0; h < m_height; h++)
	{
		for (UINT w = 0; w < m_width; w++)
		{
			COrthoTile* ot =  (COrthoTile*)m_tiles->GetElement(2, w, h);

			ot->Draw();
		}
	}
}