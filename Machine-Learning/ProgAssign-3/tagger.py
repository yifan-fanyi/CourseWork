import numpy as np

from util import accuracy
from hmm import HMM

def dict_add(mydict, key):
	if key in mydict.keys():
		mydict[key] += 1
	else:
		mydict[key] = 1
	return mydict

def dict_add1(mydict, key1, key2):
	if key1 in mydict.keys():
		if key2 in mydict[key1].keys():
			mydict[key1][key2] += 1
		else:
			mydict[key1][key2] = 1
	else:
		mydict[key1] = {}
		mydict[key1][key2] = 1
	return mydict

# TODO:
def model_training(train_data, tags):
	"""
	Train HMM based on training data

	Inputs:
	- train_data: (1*num_sentence) a list of sentences, each sentence is an object of line class
	- tags: (1*num_tags) a list of POS tags

	Returns:
	- model: an object of HMM class initialized with parameters(pi, A, B, obs_dict, state_dict) you calculated based on train_data
	"""
	###################################################
	words_set = []
	word2pos = {}
	trans = {}
	tag_ct = {}
	first_word_ct = {}
	trans_ct = {}
	S = len(tags)
	for sentence in train_data:
		ps = ''
		first_word_ct = dict_add(first_word_ct, sentence.tags[0])
		for i,word in enumerate(sentence.words):
			if word not in words_set:
				words_set.append(word)
			tag = sentence.tags[i]
			tag_ct = dict_add(tag_ct, tag)
			word2pos = dict_add1(word2pos, tag, word)
			trans = dict_add1(trans, ps, tag)
			trans_ct = dict_add(trans_ct, ps)
			ps = tag
	obs_dict = {}
	for i, state in enumerate(words_set):
	    obs_dict[state] = i
	state_dict = {}
	for i,tag in enumerate(tags):
		state_dict[tag] = i
	pi = np.zeros(S)
	L = len(words_set)
	A = np.zeros([S, S])
	B = np.zeros((S, L))
	for tag in tags:
		try:
			pi[state_dict[tag]] = first_word_ct[tag] / len(train_data)
		except:
			pass
		for n_tag in tags:
			try:
				A[state_dict[tag]][state_dict[n_tag]] = trans[tag][n_tag] / trans_ct[tag]
			except:
				pass
		for word, count in word2pos[tag].items():
			B[state_dict[tag]][obs_dict[word]] = count / tag_ct[tag]
		model = HMM(pi, A, B, obs_dict, state_dict)
	return model

# TODO:
def sentence_tagging(test_data, model, tags):
	"""
	Inputs:
	- test_data: (1*num_sentence) a list of sentences, each sentence is an object of line class
	- model: an object of HMM class

	Returns:
	- tagging: (num_sentence*num_tagging) a 2D list of output tagging for each sentences on test_data
	"""
	tagging = []
	###################################################
	for line in test_data:
		ct = 0
		for word in line.words:
			if word not in model.obs_dict:
				model.obs_dict[word] = len(model.obs_dict)
				ct += 1
		if ct:
			model.B = np.hstack([model.B, np.full([len(tags), ct], 1e-10)])
		tagging.append(model.viterbi(line.words))
	###################################################
	return tagging
