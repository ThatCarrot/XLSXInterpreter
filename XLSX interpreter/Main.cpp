#include "Functions.h"

int main() {

	std::vector<std::string>* files_t1_2023 = getXlsxFiles("files/type1");
	std::ofstream logFile_t1("log_t1.txt");
	std::ofstream outputFile_t1("output_t1.txt");
	std::string finalString_t1 = "INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, note) \n VALUES \n";

	std::vector<std::string>* files_t2_2022 = getXlsxFiles("files/type2");
	std::ofstream logFile_t2("log_t2.txt");
	std::ofstream outputFile_t2("output_t2.txt");
	std::string finalString_t2 = "INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, note) \n VALUES \n";

	for (std::string file : *files_t1_2023) {
		
		std::cout << file << "\n";

		// get all entries
		std::vector<std::string>* entries = getEntries_t1(file, logFile_t1);

		// compose finalString
		for (std::string entry : *entries) {
			finalString_t1 += entry;
		}

		// delete vector
		delete entries;
	}
	outputFile_t1 << finalString_t1;
	outputFile_t1.close();
	logFile_t1.close();

	for (std::string file : *files_t2_2022) {

		std::cout << file << "\n";

		// get all entries
		std::vector<std::string>* entries = getEntries_t2(file, logFile_t2);

		// compose finalString
		for (std::string entry : *entries) {
			finalString_t2 += entry;
		}

		// delete vector
		delete entries;
	}
	outputFile_t2 << finalString_t2;
	outputFile_t2.close();
	logFile_t2.close();

	system("pause");
	return 0;
}

// INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, site_id, product_id, note)
// VALUES('2023-09-27 10:00:00', '2023-09-27 12:30:00', 'http://www.example.com', 'Luogo di esempio', 99.99, 'Titolo di esempio', 1, 1, 'Nota di esempio');
