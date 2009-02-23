#ifndef CMDLINEPARSER_H_
#define CMDLINEPARSER_H_

#include "CairoTTY.h"

class CmdLineParser
{
public:
	CmdLineParser(int argc, char* const argv[]);
	const PageSize &GetPageSize() const;
	bool GetLandscape() const;
	const std::string &GetOutputFile() const;
	const std::string &GetInputFile() const;
	ICharPreprocessor *GetPreprocessor() const;
	const std::string &GetFontFace() const;
	double GetFontSize() const;
	
protected:
	void SetPageSize(const char *arg);
	void SetPreprocessor(const char *arg);
	void SetFontFace(const char *arg);
	void SetFontSize(const char *arg);
	
	void PrintHelp();

private:
	static const struct option long_options[];
	static const char *short_options;
	
	const std::string m_ProgName;
	
	PageSize	m_PageSize;
	bool		m_Landscape;
	ICharPreprocessor *m_Preprocessor;
	std::string m_OutputFile;
	bool m_OutputFileSet;
	std::string m_InputFile;
	std::string m_FontFace;
	double m_FontSize;
};

#endif /*CMDLINEPARSER_H_*/
