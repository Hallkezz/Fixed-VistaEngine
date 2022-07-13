#include "stdafx.h"
#include "UnitListExportExcel.h"
#include "ExcelExport\ExcelExporter.h"
#include "Units\UnitAttribute.h"
#include "Serialization\Dictionary.h"
#include "Serialization\StringTable.h"
#include "UnicodeConverter.h"

UnitListExportExcel::UnitListExportExcel(const char* filename)
{
	application_ = ExcelExporter::create(filename);
	application_->beginSheet();
}

UnitListExportExcel::~UnitListExportExcel()
{
	application_->free();
}


void UnitListExportExcel::exportUnitList()
{
	application_->setCellText(Vect2i(0,0), a2w(TRANSLATE("��� �����/��� �����")).c_str());

	int column = 0;
	int row = 0;
	AttributeLibrary::Map::const_iterator mi;
	FOR_EACH(AttributeLibrary::instance().map(), mi){
		column++;
		row++;
		const AttributeBase* attribute = mi->get();

		application_->setCellText(Vect2i(column, 0), a2w(attribute->libraryKey()).c_str());
		// formatting
		application_->setCellTextOrientation(Vect2i(column, 0), -90);
		application_->setColumnWidthAuto(column);

		application_->setCellText(Vect2i(0, row), a2w(attribute->libraryKey()).c_str());
		
	}	

	application_->setColumnWidthAuto(0);
}
