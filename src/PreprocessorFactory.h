#ifndef PREPROCESSORFACTORY_H_
#define PREPROCESSORFACTORY_H_

#include "CairoTTY.h"

class PreprocessorFactory
{
public:
    static void Print(std::ostream &s);
    static ICharPreprocessor* Lookup(const char *name);
    static ICharPreprocessor* GetDefault();

protected:
    PreprocessorFactory()
    {}

private:
    struct PreprocessorItem
    {
        const char *name;
        ICharPreprocessor *preproc;
    };

    static const PreprocessorItem m_Preprocessors[];
};

#endif /*PREPROCESSORFACTORY_H_*/
