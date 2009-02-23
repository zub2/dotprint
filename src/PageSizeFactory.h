#ifndef PAGESIZEFACTORY_H_
#define PAGESIZEFACTORY_H_

#include "CairoTTY.h"

/*
 * 1 inch = 2.54 centimeters
 * 1 point = 1/72.0 inch
 * 
 * 72.0 points =2.54 cm
 * 
 * 1 cm = 72.0/2.54 points 
 */

/** \brief 1 cm in points. */
const double milimeter = 72.0/25.4; 

class PageSizeFactory
{
public:
	static void Print(std::ostream &s);
	static const PageSize *Lookup(const char *name);
	static const PageSize &GetDefault();

protected:
	PageSizeFactory()
	{}
	
private:
	struct Page
	{
		const char *name;
		PageSize page;
	};

	static const Page m_Pages[];
};

#endif /*PAGESIZEFACTORY_H_*/
