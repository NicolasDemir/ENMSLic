
#ifndef FIRST_MODULE_H
#define FIRST_MODULE_H


class CIniFilmeMgr
{
public:
    CIniFilmeMgr() {};
    string readIniEntry(const string& filePath, const string& section, const string& key);
    void writeIniEntry(const string& filePath, const string& section, const string& key, const string& value);

};
#endif // FIRST_MODULE_H
