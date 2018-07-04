# minisearch
Mini Search Engine
==================
run: ./minisearch -i docfile -k K [-sorted]
	K:results to show in '/search' command
	docfile:includes lines as "id file_content"
		(example of docfiles in "docs" folder)
	optional flag -sorted: sorts inserted words in trie

Commands:
	1) /df:prints words from all contents and their frequency
			(if given flag -sorted words here will be sorted)
	2) /df word1 word2 ... wordN:prints frequencies for those N words
	3) /tf id word:prints frequency of word 'word' in specific id 'id'
	4) /search word1 ... wordN(maxN=10):finds a score for every content
			that has at least one of the N words(using BM25) and prints
			the top-K contents with matched words underlined.
	5) /exit:frees allocated space and terminates the program
