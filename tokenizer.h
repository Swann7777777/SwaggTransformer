#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


void tokenize() {



	std::ifstream vocabularyFileIn("vocabulary.txt");

	std::vector<std::string> tokens;

	std::string line;

	while (std::getline(vocabularyFileIn, line)) {
		tokens.push_back(line);
	}







	std::ifstream corpusStream("corpus.txt");

	std::vector<std::string> words;

	std::string corpusString;


	//std::string delimiters = " ,.?!-':;()[]0123456789%=\"";




	std::map<std::string, char> specialChar = {
		{"\xC3\xA9", 'e'},
		{"\xC3\xA0", 'a'},
		{"\xC3\xA8", 'e'},
		{"\xC3\xB9", 'u'},
		{"\xC3\xAA", 'e'},
		{"\xC3\xA2", 'a'},
		{"\xC3\xA7", 'c'},
		{"\xC3\xB4", 'o'},
		{"\xC3\xAE", 'i'},
		{"\xC3\xAF", 'i'}
	};





	while (std::getline(corpusStream, corpusString)) {



		size_t pos = 0;


		std::transform(corpusString.begin(), corpusString.end(), corpusString.begin(), [](unsigned char c) { return std::tolower(c); });


		for (auto const& [special, normal] : specialChar) {
			while ((pos = corpusString.find(special)) != std::string::npos) {

				corpusString.replace(pos, special.size(), 1, normal);
			}
		}




		size_t startPos = 0;
		for (size_t i = 0; i < corpusString.size(); i++) {
			for (int j = 0; j < delimiters.size(); j++) {

				if (corpusString[i] == delimiters[j]) {
					words.push_back(corpusString.substr(startPos, i - startPos));
					startPos = i + 1;
				}
			}
		}
		words.push_back(corpusString.substr(startPos, corpusString.size() - startPos));
	}



	words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& str) {
		return str.empty();
		}), words.end());












	std::vector<std::vector<int>> tokenizedWords(words.size());

	bool match = false;

	for (int i = 0; i < words.size(); i++) {

		pos = 0;

		for (int j = words[i].size(); j > 0; j--) {

			std::string substring = words[i].substr(pos, j);


			for (int k = 0; k < tokens.size(); k++) {

				if (substring == tokens[k]) {

					pos += j;

					j = words[i].size() - pos + 1;



					tokenizedWords[i].push_back(k);



					break;
				}
			}
		}
	}


	std::vector<std::pair<int, int>> pairs;


	for (int i = 0; i < tokenizedWords.size(); i++) {
		for (int j = 0; j < tokenizedWords[i].size() - 1; j++) {
			pairs.push_back({ tokenizedWords[i][j], tokenizedWords[i][j + 1] });
			//std::cout << tokenizedWords[i][j] << " " << tokenizedWords[i][j + 1] << "	";
		}
		//std::cout << "\n";
	}



	for (auto& i : tokenizedWords) {
		for (int j = 0; j < i.size(); j++) {
			std::cout << tokens[i[j]] << " ";
		}
		std::cout << "\n";

	}



	std::map<std::pair<int, int>, int> frequencies;



	for (auto& i : pairs) {
		frequencies[i]++;
	}




	std::vector<std::pair<std::pair<int, int>, int>> orderedFrequencies;


	int max = 0;

	for (auto& i : frequencies) {
		std::pair<std::pair<int, int>, int> pair;
		pair.first = i.first;
		pair.second = i.second;
		orderedFrequencies.push_back(pair);

	}



	auto comp = [](std::pair<std::pair<int, int>, int> a, std::pair<std::pair<int, int>, int> b) {

		return a.second < b.second;
		};



	std::sort(orderedFrequencies.begin(), orderedFrequencies.end(), comp);



	std::ofstream vocabularyFileOut("vocabulary.txt", std::ios::app);


	//vocabularyFileOut << "\n" << tokens[orderedFrequencies[orderedFrequencies.size() - 1].first.first] + tokens[orderedFrequencies[orderedFrequencies.size() - 1].first.second];




	//for (auto& i : orderedFrequencies) {

	//	std::cout << tokens[i.first.first] << tokens[i.first.second] << " : " << i.second << "\n";
	//}
}