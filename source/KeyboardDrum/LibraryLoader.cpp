#include "LibraryLoader.h"
#include "RIFFReader.h"
#include <mmreg.h>


using namespace MapleDrum;
using namespace MapleDrum::Utilities;



bool MapleDrum::Utilities::LoadRawAudio(string path, RawAudio & audio)
{
	HANDLE hFile;
	hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return false;
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return false;

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &audio.format, sizeof(WAVEFORMATEX), dwChunkPosition);

	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	audio.size = dwChunkSize;
	audio.data = new byte[dwChunkSize];
	ReadChunkData(hFile, audio.data, dwChunkSize, dwChunkPosition);
	return true;
}
