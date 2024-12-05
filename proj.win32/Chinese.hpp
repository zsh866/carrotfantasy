#pragma once
#include"cocos2d.h"
#include<map>
#include<cstring>
#include<fstream>
USING_NS_CC;
class ChineseWord {
private:
	std::string _m_path_read;
	ValueMap configData;
public:
	ChineseWord(std::string path = "ChineseContent.xml") :_m_path_read(path) {
		configData = FileUtils::getInstance()->getValueMapFromFile(_m_path_read);
	}
	std::string getWord(std::string key) {
		try {
			return configData.at(key).asString();
		}
		catch (const std::out_of_range& oor) {
			CCLOG("%s", oor.what());
			return "Error,can not find the key";
		}
	}
	bool setWord(std::string key, std::string value) {
		if (configData.find(key) == configData.end())
			return false;
		configData[key] = value;
		return true;
	}
	void changePath(std::string newpath) {
		configData.clear();
		_m_path_read = newpath;
		configData = FileUtils::getInstance()->getValueMapFromFile(_m_path_read);
	}
	bool removeWord(std::string key) {
		if (configData.find(key) == configData.end())
			return false;
		configData.erase(key);
		return true;
	}
	bool insertWord(std::string key, std::string value) {
		if (configData.find(key) != configData.end())
			return false;
		configData.insert(std::pair<std::string, Value>(key, Value(value)));
		return true;
	}
	bool Store(std::string path = "") {
		if (path == "")
			path = "../Resources/" + _m_path_read;
		auto _write_object = FileUtils::getInstance()->writeValueMapToFile(configData, path);
		if (_write_object == false)
			return false;
		return true;
	}
};