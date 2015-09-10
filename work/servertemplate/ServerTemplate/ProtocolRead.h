#pragma once

inline VOID READ_PT_OVERLAP_USER_ID(BYTE *buffer, S_PT_OVERLAP_USER_ID &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadCHARs(parameter.USER_ID, 32);
}