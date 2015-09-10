#pragma once

inline DWORD WRITE_PT_OVERLAP_USER_ID_SUCC_U(BYTE *buffer, S_PT_OVERLAP_USER_ID_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}