#include "Functions.h"

// Used for first study
//std::vector<std::string>* getColummnNames(XLSheet* sheet) {
//
//    std::vector<std::string>* columnNames = new std::vector<std::string>();
//
//    //Get all column names
//    int row = sheet->firstRow();
//    for (int col = sheet->firstCol() + 1; col < sheet->lastCol(); ++col)
//    {
//        if (sheet->cellType(row, col) != CELLTYPE_BLANK) {
//            std::wstring cellName = sheet->readStr(row, col);
//            columnNames->push_back(std::string(cellName.begin(), cellName.end()));
//        }
//    }
//
//    return columnNames;
//
//}

//Book* loadFile(std::string inputFile) {
//
//    Book* book = xlCreateXMLBook();
//    std::wstring wstring(inputFile.begin(), inputFile.end());
//
//    if (!book->load(wstring.c_str())) {
//        std::cout << "Could not load file\n";
//        std::cout << book->errorMessage() << "\n";
//        return nullptr;
//    }
//
//    return book;
//
//}


std::vector<std::string>* getXlsxFiles(std::string folder) {

    std::vector<std::string>* xlsxFiles = new std::vector<std::string>();

    for (const auto& entry : std::filesystem::directory_iterator(folder, std::filesystem::directory_options::skip_permission_denied)) {

        std::string file = entry.path().generic_string();


        if (file.substr(file.find_last_of(".") + 1) != "xlsx")
            continue;

        xlsxFiles->push_back(file);

    }

    return xlsxFiles;
}

// Old Prototype
//std::vector<std::string>* XLSXInterpreter()
//{
//    std::cout << "Starting XLSX interpreter\n";
//
//    std::vector<std::string>* xlsxFiles = getXlsxFiles("files");
//
//    for (std::string file : *xlsxFiles) {
//        Book* book = loadFile(file);
//        auto columns = getColummnNames(book->getSheet(0));
//        for (std::string column : *columns) {
//            std::cout << column << "|";
//        }
//        std::cout << "\t\tfile: " << file << "\n";
//    }
//
//    return xlsxFiles;
//
//}

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
    std::cout << "\tRemaining count:\t" << Total - (Type1 + Type2) << ".\n";

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

// Not Used
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

// Not Used
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

        if (timeInfo->tm_year == year - 1900) {
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

OpenXLSX::XLDocument loadFile(std::string filepath) {
    OpenXLSX::XLDocument doc = OpenXLSX::XLDocument();
    doc.open(filepath);
    return doc;
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

// "INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, note) \n VALUES \n"
std::vector<std::string>* getEntries_t1(std::string fileName, std::ofstream& logFile, int* totalRows, int* readRows) {

    //Log
    logFile << "Opening File " + fileName << "\n";
    int ignoredRows = 0;

    //Load XLSX
    OpenXLSX::XLDocument book = loadFile(fileName);
    OpenXLSX::XLWorksheet sheet = book.workbook().sheet(1);

    //Build vector
    std::vector<std::string>* entries = new std::vector<std::string>();

    //Read file    
    for (int row = 2; row < sheet.rowCount(); row++) {

        std::string link, price, title, place, time_of_publishing, time_of_scraping, note, finalString, site_id, product_id;

        // get Time info from file
        std::string unixDateTimeStr = fileName;
        size_t pos = unixDateTimeStr.find_last_of('.');
        unixDateTimeStr = unixDateTimeStr.substr(0, pos);
        pos = unixDateTimeStr.find_last_of("_");
        unixDateTimeStr = unixDateTimeStr.substr(pos + 1);
        double unixTimestamp;
        std::istringstream(unixDateTimeStr) >> unixTimestamp;
        std::time_t timestamp = static_cast<std::time_t>(unixTimestamp);
        std::tm* tmInfo = std::localtime(&timestamp);


        // get link *
        {
            OpenXLSX::XLCell linkCell = sheet.cell(row, 2);

            if (linkCell.value().type() == OpenXLSX::XLValueType::Empty) {
                ignoredRows++;
                logInvalidRow(logFile, 1, fileName, row);
                continue;
            }

            link = linkCell.value().get<std::string>();

            link = "CONCAT(\'" + link + "\')";
        }

        // get price *
        {

            OpenXLSX::XLCell priceCell = sheet.cell(row, 3);

            if (priceCell.value().type() == OpenXLSX::XLValueType::Empty) {
                ignoredRows++;
                logInvalidRow(logFile, 2, fileName, row);
                continue;
            }

            std::string price_dirty = priceCell.value().get<std::string>();;

            if (price_dirty.find("\\n") != std::string::npos || price_dirty.find("GRATIS") != std::string::npos) {
                ignoredRows++;
                logInvalidRow(logFile, 2, fileName, row);
                continue;
            }

            for (char character : price_dirty) {
                if (character >= 0)
                    if (std::isdigit(character)) {
                        price += character;
                    }
            }
            if (price.length() == 0) {
                price = "-1";
            }

        }

        // get title *
        {

            OpenXLSX::XLCell titleCell = sheet.cell(row, 4);

            if (titleCell.value().type() == OpenXLSX::XLValueType::Empty) {
                ignoredRows++;
                logInvalidRow(logFile, 3, fileName, row);
                continue;
            }

            title = titleCell.value().get<std::string>();

            // Remove ' and "
            size_t apostrohePos = title.find('\'');
            while (apostrohePos != std::string::npos) {
                title.replace(apostrohePos, 1, 1, ' ');
                apostrohePos = title.find('\'');
            }
            size_t doubleApostrohePos = title.find('\"');
            while (doubleApostrohePos != std::string::npos) {
                title.replace(doubleApostrohePos, 1, 1, ' ');
                doubleApostrohePos = title.find('\"');
            }

            // Remove \n
            size_t returnPos = title.find('\n');
            while (returnPos != std::string::npos) {
                title.replace(returnPos, 1, 1, ' ');
                returnPos = title.find('\n');
            }


            // Have concat function, to avoid &
            title = "CONCAT(\'" + title + "\')";

        }

        // get place
        {

            OpenXLSX::XLCell placeCell = sheet.cell(row, 5);

            place = placeCell.value().get<std::string>();

            // Remove '
            size_t apostrohePos = place.find('\'');
            if (apostrohePos != std::string::npos) {
                place.replace(apostrohePos, 1, 1, ' ');
            }

            place = '\"' + place + '\"';

        }

        // get TOS
        {
            if (tmInfo != nullptr) {
                int year = tmInfo->tm_year + 1900;
                int month = tmInfo->tm_mon + 1;
                int day = tmInfo->tm_mday;
                int hour = tmInfo->tm_hour;
                int minute = tmInfo->tm_min;

                std::stringstream formattedDateTime;
                formattedDateTime << std::setfill('0') << std::setw(2) << day << "/"
                    << std::setfill('0') << std::setw(2) << month << "/"
                    << year << " " << std::setfill('0') << std::setw(2) << hour << ":"
                    << std::setfill('0') << std::setw(2) << minute;

                time_of_scraping = "STR_TO_DATE(\'" + formattedDateTime.str() + "\', \'%d/%m/%Y %H:%i\')";

            }
        }

        // get TOP
        {
            OpenXLSX::XLCell TOPCell = sheet.cell(row, 6);

            if (TOPCell.value().get<std::string>() == "")
                time_of_publishing = "_";
            else
                time_of_publishing = TOPCell.value().get<std::string>();
            
            // is place or empty, set as TOS
            if (time_of_publishing.find("(") != std::string::npos || time_of_publishing == "_") {
                time_of_publishing = time_of_scraping;
            }

            // replace date 'Oggi' or 'Ieri'
            else if (time_of_publishing.find("Oggi") != std::string::npos || time_of_publishing.find("Ieri") != std::string::npos || time_of_publishing.find("alle") != std::string::npos) {
                if (tmInfo != nullptr) {
                    int year = tmInfo->tm_year + 1900;
                    int month = tmInfo->tm_mon + 1;
                    int day = tmInfo->tm_mday;
                    int hourIndex = time_of_publishing.find("alle") + 4 + 1;
                    int hour = std::stoi(time_of_publishing.substr(hourIndex, 2));
                    int minute = std::stoi(time_of_publishing.substr(hourIndex+3, 2));

                    std::stringstream formattedDateTime;
                    formattedDateTime << std::setfill('0') << std::setw(2) << day << "/"
                        << std::setfill('0') << std::setw(2) << month << "/"
                        << year << " " << std::setfill('0') << std::setw(2) << hour << ":"
                        << std::setfill('0') << std::setw(2) << minute;

                    time_of_publishing = "STR_TO_DATE(\'" + formattedDateTime.str() + "\', \'%d/%m/%Y %H:%i\')";
                }
            }

            // is datetime format
            else {
                for (int i = 0; i < 2; i++) {
                    size_t pos = time_of_publishing.find('-');
                    if (pos != std::string::npos) {
                        time_of_publishing.replace(pos, 1, 1, '/');

                    }
                }
                if (std::count(time_of_publishing.begin(), time_of_publishing.end(), ':') > 1) {
                    time_of_publishing = time_of_publishing.substr(0, 16);
                }
                if (time_of_publishing.find('/') > 3) { // check if year is first or day
                    time_of_publishing = "STR_TO_DATE(\'" + time_of_publishing + "\', \'%Y/%m/%d %H:%i\')";
                }
                else {
                    time_of_publishing = "STR_TO_DATE(\'" + time_of_publishing + "\', \'%d/%m/%Y %H:%i\')";
                }
            }
        }

        // get note
        {
            note = fileName;
            note += ":" + std::to_string(row);
            note = '\'' + note + '\'';
        }

        // compose finalString
        finalString += '(' + time_of_publishing + ',' + time_of_scraping + ',' + link + ',' + place + ',' + price + ',' + title + ',' + note + ")";

        // push into vector
        entries->push_back(finalString);


    }

    *totalRows += sheet.rowCount();
    *readRows += sheet.rowCount() - ignoredRows;

    book.close();

    return entries;
}

// "INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, note) \n VALUES \n"
std::vector<std::string>* getEntries_t2(std::string fileName, std::ofstream& logFile, int* totalRows, int* readRows) {

    //Log
    logFile << "Opening File " + fileName << "\n";
    int ignoredRows = 0;

    //Load XLSX
    OpenXLSX::XLDocument book = loadFile(fileName);
    OpenXLSX::XLWorksheet sheet = book.workbook().sheet(1);

    //Build vector
    std::vector<std::string>* entries = new std::vector<std::string>();

    //Read file    
    for (int row = 2; row < sheet.rowCount(); row++) {

        std::string link, price, title, place, time_of_publishing, time_of_scraping, note, finalString;

        // get Time info from file
        size_t posFwSlash = fileName.rfind('/');
        size_t posPunto = fileName.rfind('.');

        // get TOS *
        if (posFwSlash != std::string::npos && posPunto != std::string::npos) {
            std::string dateTime = fileName.substr(posFwSlash + 1, posPunto - posFwSlash - 4);
            dateTime.replace(2, 1, "/");
            dateTime.replace(5, 1, "/");
            dateTime.replace(13, 1, ":");
            //STR_TO_DATE('09-05-2023 21:56', '%d-%m-%Y %H:%i')
            time_of_scraping = "STR_TO_DATE(\'" + dateTime + "\', \'%d/%m/%Y %H:%i\')";
        }

        // get link *
        {

            OpenXLSX::XLCell linkCell = sheet.cell(row, 5);

            if (linkCell.value().type() == OpenXLSX::XLValueType::Empty) {
                ignoredRows++;
                logInvalidRow(logFile, 1, fileName, row);
                continue;
            }

            link = linkCell.value().get<std::string>();

            link = "CONCAT(\'" + link + "\')";
        }

        // get price *
        {
            OpenXLSX::XLCell priceCell = sheet.cell(row, 4);

            if (priceCell.value().type() == OpenXLSX::XLValueType::Empty) {
                ignoredRows++;
                logInvalidRow(logFile, 2, fileName, row);
                continue;
            }

            std::string price_dirty = priceCell.value().get<std::string>();;

            if (price_dirty.find("\\n") != std::string::npos || price_dirty.find("GRATIS") != std::string::npos) {
                logInvalidRow(logFile, 2, fileName, row);
                continue;
            }

            for (char character : price_dirty) {
                if (character >= 0)
                    if (std::isdigit(character)) {
                        price += character;
                    }
            }
            if (price.length() == 0) {
                price = "-1";
            }

        }

        // get title *
        {
            OpenXLSX::XLCell titleCell = sheet.cell(row, 1);

            if (titleCell.value().type() == OpenXLSX::XLValueType::Empty) {
                ignoredRows++;
                logInvalidRow(logFile, 3, fileName, row);
                continue;
            }

            title = titleCell.value().get<std::string>();

            // Remove ' and "
            size_t apostrohePos = title.find('\'');
            while (apostrohePos != std::string::npos) {
                title.replace(apostrohePos, 1, 1, ' ');
                apostrohePos = title.find('\'');
            }
            size_t doubleApostrohePos = title.find('\"');
            while (doubleApostrohePos != std::string::npos) {
                title.replace(doubleApostrohePos, 1, 1, ' ');
                doubleApostrohePos = title.find('\"');
            }

            // Remove \n
            size_t returnPos = title.find('\n');
            while (returnPos != std::string::npos) {
                title.replace(returnPos, 1, 1, ' ');
                returnPos = title.find('\n');
            }

            // Have concat function, to avoid &
            title = "CONCAT(\'" + title + "\')";
            

        }

        // get place
        {

            OpenXLSX::XLCell placeCell = sheet.cell(row, 2);

            place = placeCell.value().get<std::string>();

            // Remove '
            size_t apostrohePos = place.find('\'');
            if (apostrohePos != std::string::npos) {
                place.replace(apostrohePos, 1, 1, ' ');
            }

            place = '\"' + place + '\"';
        }

        // get TOP
        {
            OpenXLSX::XLCell TOPCell = sheet.cell(row, 3);
            time_of_publishing = time_of_scraping;

        }

        // get note
        {
            note = fileName;
            note += ":" + std::to_string(row);
            note = '\"' + note + '\"';
        }

        // compose finalString
        finalString += '(' + time_of_publishing + ',' + time_of_scraping + ',' + link + ',' + place + ',' + price + ',' + title + ',' + note + ")";

        // push into vector
        entries->push_back(finalString);

    }

    *totalRows += sheet.rowCount();
    *readRows += sheet.rowCount() - ignoredRows;

    return entries;
}

void logInvalidRow(std::ofstream& logFile, int state, std::string filename, int row) {

    logFile << "Ignored row " << row;
    switch (state) {
    default:
        logFile << " -Unhandled\n";
        break;
    case 1:
        logFile << " -Missing link\n";
        break;
    case 2:
        logFile << " -Missing price\n";
        break;
    case 3:
        logFile << " -Missing title\n";
        break;
    }

}