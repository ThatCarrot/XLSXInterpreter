#include "Functions.h"

int main() {
	
	std::string baseString = "INSERT INTO cvp.nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, note) \n VALUES \n";

	//type 1
	std::ofstream logFile_t1("../output/log_t1.txt");
	std::ofstream outputFile_t1("../output/output_t1.txt");

	std::vector<std::string>* files_t1_2023 = getXlsxFiles("../files/type1");
	for (std::string file : *files_t1_2023) {
		outputFile_t1 << baseString;
		
		// print what file we're working with
		std::cout << file << "\n";

		// get all entries
		std::vector<std::string>* entries = getEntries_t1(file, logFile_t1);

		// compose finalString
		for (int i = 0; i < entries->size(); i++) {
			std::string entry = entries->at(i);
			outputFile_t1 << entry;
			if (i < entries->size() - 1)
				outputFile_t1 << ",\n";
			else
				outputFile_t1 << ";\n\n";
		}

		// delete vector
		delete entries;
	}
	outputFile_t1.close();
	logFile_t1.close();

	//type 2
	std::ofstream logFile_t2("../output/log_t2.txt");
	std::ofstream outputFile_t2("../output/output_t2.txt");

	std::vector<std::string>* files_t2_2022 = getXlsxFiles("../files/type2");
	for (std::string file : *files_t2_2022) {
		outputFile_t2 << baseString;

		// print what file we're working with
		std::cout << file << "\n";

		// get all entries
		std::vector<std::string>* entries = getEntries_t2(file, logFile_t2);

		// compose finalString
		for (int i = 0; i < entries->size(); i++) {
			std::string entry = entries->at(i);
			outputFile_t2 << entry;
			if (i < entries->size() - 1)
				outputFile_t2 << ",\n";
			else
				outputFile_t2 << ";\n\n";
		}

		// delete vector
		delete entries;
	}
	outputFile_t2.close();
	logFile_t2.close();

	system("pause");
	return 0;
}

// INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, site_id, product_id, note)
// VALUES('2023-09-27 10:00:00', '2023-09-27 12:30:00', 'http://www.example.com', 'Luogo di esempio', 99.99, 'Titolo di esempio', 1, 1, 'Nota di esempio');
