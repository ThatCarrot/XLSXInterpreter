#include "Functions.h"

int main() {

	std::vector<std::string>* files_t1_2023 = getXlsxFiles("files/type1/2023");

	std::vector<std::string>* files_t2_2022 = getXlsxFiles("files/type2/2022");
	std::vector<std::string>* files_t2_2023 = getXlsxFiles("files/type2/2023");

	for (std::string line : *files_t2_2022) {
		std::cout << line << "\n";
	}

	std::vector<std::string> commands;

	commands.push_back(XLSXExtractor_t1(files_t1_2023));

	std::ofstream file("output_t1_2023.txt");
	
	if (!file.is_open()) {
		std::cerr << "Impossibile aprire il file." << std::endl;
		return 1; // Termina il programma con un codice di errore
	}

	file << commands.at(0);

	file.close();

	system("pause");
	return 0;
}

// INSERT INTO nome_tabella (time_of_publishing, time_of_scraping, link, place, prezzo, titolo, site_id, product_id, note)
// VALUES('2023-09-27 10:00:00', '2023-09-27 12:30:00', 'http://www.example.com', 'Luogo di esempio', 99.99, 'Titolo di esempio', 1, 1, 'Nota di esempio');
