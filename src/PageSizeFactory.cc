#include <string.h>
#include "PageSizeFactory.h"

void PageSizeFactory::Print(std::ostream &s)
{
    int i = 0;
    while (m_Pages[i].name != NULL)
    {
        s << m_Pages[i].name;
        if (i == 0)
            s << " [default]";
        s << std::endl;
        ++i;
    }
}

const PageSize *PageSizeFactory::Lookup(const char *name)
{
    int i = 0;

    while (m_Pages[i].name != NULL && strcmp(m_Pages[i].name, name))
        ++i;

    if (m_Pages[i].name == NULL)
        return NULL;

    return &m_Pages[i].page;
}

const PageSize &PageSizeFactory::GetDefault()
{
    return m_Pages[0].page;
}

const PageSizeFactory::Page PageSizeFactory::m_Pages[] =
{
    { "A4", PageSize(210.0 * milimeter, 297.0 * milimeter) },
    { "A5", PageSize(148.5 * milimeter, 210.0 * milimeter) },
    { 0, PageSize(0.0, 0.0) }
};
