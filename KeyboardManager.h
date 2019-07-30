#pragma once

#include <memory>
#include <vector>

class KeyboardManager
{
public:
	static std::unique_ptr<KeyboardManager>& Instance()
	{
		static std::unique_ptr<KeyboardManager> _instance(new KeyboardManager);
		return _instance;
	}

	void getKeys();
	bool checkKey(char k);

private:
	KeyboardManager() {};

public:


private:

	std::vector<bool> keys;


};