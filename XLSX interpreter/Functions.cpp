#include "Functions.h"

std::vector<std::string>* getColummnNames(Sheet* sheet) {

    std::vector<std::string>* columnNames = new std::vector<std::string>();

    //Get all column names
    int row = sheet->firstRow();
    for (int col = sheet->firstCol() + 1; col < sheet->lastCol(); ++col)
    {
        if (sheet->cellType(row, col) != CELLTYPE_BLANK) {
            std::wstring cellName = sheet->readStr(row, col);
            columnNames->push_back(std::string(cellName.begin(), cellName.end()));
        }
    }

    return columnNames;

}

Book* loadFile(std::string inputFile) {

    Book* book = xlCreateXMLBook();
    std::wstring wstring(inputFile.begin(), inputFile.end());

    if (!book->load(wstring.c_str())) {
        std::cout << "Could not load file\n";
        std::cout << book->errorMessage() << "\n";
        return nullptr;
    }

    return book;

}


std::vector<std::string>* getXlsxFiles(std::string folder) {

    std::vector<std::string>* xlsxFiles = new std::vector<std::string>();

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        std::string file = entry.path().generic_string();

        if (file.substr(file.find_last_of(".") + 1) != "xlsx")
            continue;

        xlsxFiles->push_back(file);

    }

    return xlsxFiles;
}

std::vector<std::string>* XLSXInterpreter()
{
    std::cout << "Starting XLSX interpreter\n";

    std::vector<std::string>* xlsxFiles = getXlsxFiles("files");

    for (std::string file : *xlsxFiles) {
        Book* book = loadFile(file);
        auto columns = getColummnNames(book->getSheet(0));
        for (std::string column : *columns) {
            std::cout << column << "|";
        }
        std::cout << "\t\tfile: " << file << "\n";
    }

    return xlsxFiles;

}

int XLSXGrouper(std::vector<std::string>* xlsxFiles) {

    std::cout << ">> Starting XLSX file grouping. <<\n";
    std::cout << "Type 1: export_**********.*******.\n";
    std::cout << "Type 2: DD-MM-YYYY HH-MI-SS.\n";

    // Separate the files based on format
    std::cout << "\n>Separating by type.\n";
    std::vector<std::string>* XLSXtype1 = filterType1(xlsxFiles);
    std::vector<std::string>* XLSXtype2 = filterType2(xlsxFiles);

        int Total, Type1, Type2;
        Total = xlsxFiles->size();
        Type1 = XLSXtype1->size();
        Type2 = XLSXtype2->size();

        std::cout << "\tTotal count: \t\t" << Total << ".\n";
        std::cout << "\tType 1 count: \t\t" << Type1 << ".\n";
        std::cout << "\tType 2 count: \t\t" << Type2 << ".\n";
        std::cout << "\tRemaining count:\t" << Total-(Type1+Type2) << ".\n";

    // Move in folder based on type
    std::cout << "\n>Creating type folders.\n";
    std::filesystem::create_directories("files/type1");
    std::filesystem::create_directories("files/type2");

    std::cout << "\n>Moving files.\n";
    for (int i = 0; i < XLSXtype1->size(); i++) {
        std::string fileFrom = XLSXtype1->at(i);
        std::string fileTo = "files/type1/" + fileFrom.substr(6);
        std::filesystem::copy(fileFrom, fileTo);
        std::filesystem::remove(fileFrom);
    }

    for (int i = 0; i < XLSXtype2->size(); i++) {
        std::string fileFrom = XLSXtype2->at(i);
        std::string fileTo = "files/type2/" + fileFrom.substr(6);
        std::filesystem::copy(fileFrom, fileTo);
        std::filesystem::remove(fileFrom);
    }

    delete XLSXtype1;
    delete XLSXtype2;

    // Separate the files based on year
    std::cout << "\n>Separating by year.\n";
    XLSXtype1 = getXlsxFiles("files/type1");
    XLSXtype2 = getXlsxFiles("files/type2");

    auto group2022t1 = groupByYear_T1(XLSXtype1, 2022);
    auto group2023t1 = groupByYear_T1(XLSXtype1, 2023);
    auto group2022t2 = groupByYear_T2(XLSXtype2, 2022);
    auto group2023t2 = groupByYear_T2(XLSXtype2, 2023);

        int y2022t1, y2023t1, y2022t2, y2023t2;
        y2022t1 = group2022t1->size();
        y2023t1 = group2023t1->size();
        y2022t2 = group2022t2->size();
        y2023t2 = group2023t2->size();

        std::cout << "\tType 1 :" << y2022t1 << "(2022) + " << y2023t1 << "(2023).\n";
        std::cout << "\tType 2 :" << y2022t2 << "(2022) + " << y2023t2 << "(2023).\n";

    // Move in folder based on year
    std::cout << "\n>Creating year folders.\n";
    std::filesystem::create_directories("files/type1/2022");
    std::filesystem::create_directories("files/type1/2023");
    std::filesystem::create_directories("files/type2/2022");
    std::filesystem::create_directories("files/type2/2023");

    std::cout << "\n>Moving files.\n";
    for (int i = 0; i < group2022t1->size(); i++) {
        std::string fileFrom = group2022t1->at(i);
        std::string fileTo = "files/type1/2022/" + fileFrom.substr(12);
        std::filesystem::copy(fileFrom, fileTo);
        std::filesystem::remove(fileFrom);

    }
    for (int i = 0; i < group2023t1->size(); i++) {
        std::string fileFrom = group2023t1->at(i);
        std::string fileTo = "files/type1/2023/" + fileFrom.substr(12);
        std::filesystem::copy(fileFrom, fileTo);
        std::filesystem::remove(fileFrom);
    }
    for (int i = 0; i < group2022t2->size(); i++) {
        std::string fileFrom = group2022t2->at(i);
        std::string fileTo = "files/type2/2022/" + fileFrom.substr(12);
        std::filesystem::copy(fileFrom, fileTo);
        std::filesystem::remove(fileFrom);
    }
    for (int i = 0; i < group2023t2->size(); i++) {
        std::string fileFrom = group2023t2->at(i);
        std::string fileTo = "files/type2/2023/" + fileFrom.substr(12);
        std::filesystem::copy(fileFrom, fileTo);
        std::filesystem::remove(fileFrom);
    }

    return 0;
}

std::vector<std::string>* filterType1(std::vector<std::string>* input) {
    // sample: <files/export_1683662202.2087388.xlsx>

    std::vector<std::string>* result = new std::vector<std::string>();

    for (int i = 0; i < input->size(); i++) {
        std::string fileName = input->at(i);
        if (fileName.substr(6, 7) == "export_") {
            result->push_back(fileName);
        }
    }

    return result;


}

std::vector<std::string>* filterType2(std::vector<std::string>* input) {
    // sample: <files/31-10-2022 12-14-01.xlsx> (DD-MM-YYYY)

    std::vector<std::string>* result = new std::vector<std::string>();

    std::regex dateRegex("files/\\d{2}-\\d{2}-\\d{4} \\d{2}-\\d{2}-\\d{2}.xlsx");

    for (int i = 0; i < input->size(); i++) {
        std::string fileName = input->at(i);
        if (std::regex_match(fileName, dateRegex)) {
            result->push_back(fileName);
        }
    }

    return result;
}

std::vector<std::string>* groupByMonth_T1(std::vector<std::string>* input, int month) {
    // sample: <files/export_1683662202.2087388.xlsx>

    std::vector<std::string>* result = new std::vector<std::string>();

    for (int i = 0; i < input->size(); i++) {
        std::string fileName = input->at(i);

        std::string unixDateTimeStr = fileName.substr(13, 18);
        double timestamp = std::stod(unixDateTimeStr);
        std::time_t timestampTime = static_cast<std::time_t>(timestamp);
        std::tm* timeInfo = std::gmtime(&timestampTime);

        if (timeInfo->tm_mon + 1 == month) {
            result->push_back(fileName);
        }
        
    }

    return result;

}

std::vector<std::string>* groupByMonth_T2(std::vector<std::string>* input, int month) {
    // sample: <files/type2/31-10-2022 12-14-01.xlsx> (DD-MM-YYYY)

    std::vector<std::string>* result = new std::vector<std::string>();

    for (int i = 0; i < input->size(); i++) {
        std::string fileName = input->at(i);
        if (std::stoi(fileName.substr(15, 2)) == month) {
            result->push_back(fileName);
        }
    }

    return result;

}

std::vector<std::string>* groupByYear_T1(std::vector<std::string>* input, int year) {
    // sample: <files/type1/export_1683662202.2087388.xlsx>

    std::vector<std::string>* result = new std::vector<std::string>();
    
    for (int i = 0; i < input->size(); i++) {
        std::string fileName = input->at(i);

        std::string unixDateTimeStr = fileName.substr(19, 18);
        double timestamp = std::stod(unixDateTimeStr);
        std::time_t timestampTime = static_cast<std::time_t>(timestamp);
        std::tm* timeInfo = std::gmtime(&timestampTime);

        if (timeInfo->tm_year  == year - 1900) {
            result->push_back(fileName);
        }

    }

    return result;
}

std::vector<std::string>* groupByYear_T2(std::vector<std::string>* input, int year) {
    // sample: <files/type2/31-10-2022 12-14-01.xlsx> (DD-MM-YYYY)

    std::vector<std::string>* result = new std::vector<std::string>();

    for (int i = 0; i < input->size(); i++) {
        std::string fileName = input->at(i);
        if (std::stoi(fileName.substr(18, 4)) == year) {
            result->push_back(fileName);
        }
    }

    return result;
}

// INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, site_id, product_id, note)
// VALUES('2023-09-27 10:00:00', '2023-09-27 12:30:00', 'http://www.example.com', 'Luogo di esempio', 99.99, 'Titolo di esempio', 1, 1, 'Nota di esempio');


std::string XLSXExtractor_t1(std::vector<std::string>* xlsxFiles) {
    // filename     files/typex/YYYY/export_XXXXXXXXXX.XXXXXXX.xlsx
    // xlsx format  ID|Url|Price|Title|Place|TOP|Link_id|ExportStatus|

    std::string finalString = 
        "INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, site_id, product_id, note) \n VALUES \n";

    for (std::string file : *xlsxFiles) {
        Book* book = loadFile(file);
        Sheet* sheet = book->getSheet(0);

        std::cout << "Reading " << file << "\n";
        std::cout << "lastRow: " << sheet->lastRow() << "\n";

        for (int row = 1; row < sheet->lastRow(); row++) {
            try {
                std::string time_of_publishing, time_of_scraping, link, place, price, title, site_id, product_id, note;
                std::wstring wStr;

                // get link
                wStr = sheet->readStr(row, 1) == NULL ? L"_" : sheet->readStr(row, 1);
                link = '\'' + std::string(wStr.begin(), wStr.end()) + '\'';

                // get price
                wStr = sheet->readStr(row, 2) == NULL ? L"_" : sheet->readStr(row, 2);
                price = '\'' + std::string(wStr.begin(), wStr.end()) + '\'';

                // get title
                wStr = sheet->readStr(row, 3) == NULL ? L"_" : sheet->readStr(row, 3);
                title = '\'' + std::string(wStr.begin(), wStr.end()) + '\'';

                // get place
                wStr = sheet->readStr(row, 4) == NULL ? L"_" : sheet->readStr(row, 4);
                place = '\'' + std::string(wStr.begin(), wStr.end()) + '\'';

                // get TOP
                wStr = sheet->readStr(row, 5) == NULL ? L"_" : sheet->readStr(row, 5);
                time_of_publishing = '\'' + std::string(wStr.begin(), wStr.end()) + '\'';

                // get TOS
                std::string unixDateTimeStr = extractUnixDateTime(file);
                time_of_scraping = '\'' + unixDateTimeStr + '\'';

                // get link_id
                wStr = sheet->readStr(row, 6) == NULL ? L"_" : sheet->readStr(row, 6);
                note = "old link_id:" + '\'' + std::string(wStr.begin(), wStr.end()) + "\' ";
                note += "file:" + '\'' + file + '\'';

                // compose finalString
                finalString += '(' + time_of_publishing + ',' + time_of_scraping + ',' + link + ',' + place + ',' + price + ',' + title + ',' + site_id + ',' + product_id + ',' + note + "),\n";
            }
            catch (std::exception& exc) {
                std::cout << "Exception: " << exc.what() << "\n";
                std::cout << "LibXL Error Message: " << book->errorMessage() << "\n";
            }
            catch (...) {
                std::cout << "LibXL Error Message: " << book->errorMessage() << "\n";
            }
        }

    }

    return finalString;

}

std::string XLSXExtractor_t2(std::vector<std::string>* files) {
    return "";
}

std::string extractUnixDateTime(const std::string& inputString) {
    size_t posUnderscore = inputString.rfind('_');
    size_t posPunto = inputString.rfind('.');

    if (posUnderscore != std::string::npos && posPunto != std::string::npos) {
        std::string risultato = inputString.substr(posUnderscore + 1, posPunto - posUnderscore - 1);
        return risultato;
    }
    else {
        return inputString;
    }
}