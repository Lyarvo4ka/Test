#ifndef _EPSFILE
#define _EPSFILE

#define EPS_MAGIC					(DWORD) 0xC6D3D0C5

#pragma pack(push)
#pragma pack(1)
typedef struct _EPSHEADER {
	DWORD magic;					// EPS_MAGIC
	DWORD ps_offs;					// PostScript language code section.
	DWORD ps_size;					//
	DWORD metafile_offs;			// Metafile screen representation section.
	DWORD metafile_size;			//
	DWORD tiff_offs;				// TIFF section.
	DWORD tiff_size;				//
	DWORD chksum;					// Checksum of header (XOR of bytes 0-27).
									// NOTE: if Checksum is FFFF then it is to be ignored.
	_EPSHEADER(void) {};
	_EPSHEADER(_EPSHEADER *header) {
		magic = header->magic;
		ps_offs = header->ps_offs;
		ps_size = header->ps_size;
		metafile_offs = header->metafile_offs;
		metafile_size = header->metafile_size;
		tiff_offs = header->tiff_offs;
		tiff_size = header->tiff_size;
		chksum = header->chksum;
	}
} EPSHEADER, *PEPSHEADER;
#pragma pack(pop)

class EPSFile : public FileEx, EPSHEADER
{
public:
	EPSFile(TCHAR *name, PEPSHEADER header);
	static BOOL IsHeader(PEPSHEADER header);
	LONGLONG GetSize(void);

	String GetTitle();
	String GetCreationDate();
	
	~EPSFile(void);
};

#endif _EPSFILE
