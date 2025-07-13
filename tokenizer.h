#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>




class trieNode {
public:

	trieNode* children[26];


	int index;

	trieNode() {

		index = -1;

		for (int i = 0; i < 26; i++) {
			children[i] = nullptr;
		}
	}
};




std::vector<std::string> loadVocabulary() {

	std::ifstream vocabularyFileIn("vocabulary.txt");

	std::vector<std::string> vocabulary;

	std::string line;

	while (std::getline(vocabularyFileIn, line)) {
		vocabulary.push_back(line);
	}



	return vocabulary;
}






void buildTrie(std::vector<std::string> vocabulary, trieNode* root) {

	for (int i = 0; i < vocabulary.size(); i++) {

		trieNode* node = root;

		for (int j = 0; j < vocabulary[i].size(); j++) {

			if (node->children[vocabulary[i][j] - 'a'] == nullptr) {


				trieNode* newNode = new trieNode();

				node->children[vocabulary[i][j] - 'a'] = newNode;

			}

			node = node->children[vocabulary[i][j] - 'a'];

		}

		node->index = i;

	}

}





std::string loadCorpus(std::ifstream& corpusStream) {


	std::string corpusString;


	std::string line;



	bool match = false;



	while (!match) {


		std::getline(corpusStream, line);


		int pos = line.find("<text");

		if (pos != std::string::npos) {

			while (!match) {


				std::getline(corpusStream, line);

				
				int pos = line.find("</text");

				if (pos != std::string::npos) {
					match = true;
				}

				else {
					corpusString += line;
				}

			}



		}


	}


	return corpusString;

}




std::vector<std::string> normalizeCorpus(std::string corpusString) {


	std::vector<std::string> words;



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
	{"\xC3\xAF", 'i'},
	{"\xC3\xBB", 'u'}

	};








	size_t pos = 0;


	std::transform(corpusString.begin(), corpusString.end(), corpusString.begin(), [](unsigned char c) { return std::tolower(c); });



	for (auto const& [special, normal] : specialChar) {
		while ((pos = corpusString.find(special)) != std::string::npos) {

			corpusString.replace(pos, special.size(), 1, normal);
		}
	}


	size_t startPos = 0;
	for (size_t i = 0; i < corpusString.size(); i++) {


		if (corpusString[i] <= -1 || !std::isalpha(corpusString[i])) {

			words.push_back(corpusString.substr(startPos, i - startPos));
			startPos = i + 1;
		}
	}

	words.push_back(corpusString.substr(startPos, corpusString.size() - startPos));






	words.erase(
		std::remove_if(words.begin(), words.end(), [](const std::string& str) {
			return str.empty() || str.size() == 1;
			}),
		words.end()
	);





	return words;

}
























std::vector<std::vector<int>> tokenizeCorpus(std::vector<std::string> words, trieNode* root) {

	std::vector<std::vector<int>> tokenizedWords(words.size());



	for (int i = 0; i < words.size(); i++) {


		for (int j = 0; j < words[i].size();) {


			size_t startPos = j;

			std::string subWord = words[i].substr(j, words[i].size() - j);

			trieNode* node = root;


			for (int k = 0; k <= subWord.size(); k++) {




                if (node->children[subWord[k] - 'a'] == nullptr) {



					tokenizedWords[i].push_back(node->index);


					node = root;
				}


				j++;


				node = node->children[subWord[k] - 'a'];


			}
		}
	}



	return tokenizedWords;
}




std::vector<std::pair<int, int>> createPairs(std::vector<std::vector<int>> tokenizedWords) {

	std::vector<std::pair<int, int>> pairs;


	for (int i = 0; i < tokenizedWords.size(); i++) {

		for (int j = 0; j < tokenizedWords[i].size() - 1; j++) {

			pairs.push_back({ tokenizedWords[i][j], tokenizedWords[i][j + 1] });
		}
	}

	return pairs;
}





std::map<std::pair<int, int>, int> calculateFrequencies(std::vector<std::pair<int, int>> pairs) {
	
	std::map<std::pair<int, int>, int> frequencies;



	for (auto& i : pairs) {
		frequencies[i]++;
	}

	return frequencies;
}







std::vector<std::pair<std::pair<int, int>, int>> orderFrequencies(std::map<std::pair<int, int>, int> frequencies) {

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


	return orderedFrequencies;
}





void outputPair(std::vector<std::string> vocabulary, std::vector<std::pair<std::pair<int, int>, int>> orderedFrequencies) {

	std::ofstream vocabularyFileOut("vocabulary.txt", std::ios::app);



	vocabularyFileOut << "\n" << vocabulary[orderedFrequencies[orderedFrequencies.size() - 1].first.first] + vocabulary[orderedFrequencies[orderedFrequencies.size() - 1].first.second];

}










void tokenize() {




	std::vector<std::string> vocabulary = loadVocabulary();


	trieNode* root = new trieNode();



	buildTrie(vocabulary, root);





	std::ifstream corpusStream("C:\\Users\\SWAGG\\Downloads\\simplewiki-20250701-pages-articles-multistream.xml\\simplewiki-20250701-pages-articles-multistream.xml");






	std::map<std::pair<int, int>, int> frequencies;






	for (int i = 0; i < 1000; i++) {


		std::string corpusString = loadCorpus(corpusStream);




		std::vector<std::string> words = normalizeCorpus(corpusString);





		std::vector<std::vector<int>> tokenizedWords = tokenizeCorpus(words, root);





		std::vector<std::pair<int, int>> pairs = createPairs(tokenizedWords);





		frequencies = calculateFrequencies(pairs);






		corpusString.clear();

		words.clear();

		tokenizedWords.clear();

		pairs.clear();

	}



	std::vector<std::pair<std::pair<int, int>, int>> orderedFrequencies = orderFrequencies(frequencies);




	outputPair(vocabulary, orderedFrequencies);

}