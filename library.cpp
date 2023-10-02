#include "library.h"

#include <filesystem>
#include <fstream>
#include <string>
#include "Scanner.h"
#include "Cwsq.h"
#include "Mindtct.h"
#include "Bozorth.h"
#include "Utils.h"

namespace fs = std::filesystem;

int session_id_len = 8;
std::string session_id = Fbr::RandomString(session_id_len);
const static fs::path BASE_TMP_DIR_PATH = fs::temp_directory_path() / "fbr";
const fs::path TMP_DIR_PATH = BASE_TMP_DIR_PATH / session_id;

static FBR_MATCH_CB callback;

std::string base_templates[MAX_TEMPLATES];
std::string last_error;
int template_count = 0;

int FBRInit()
{
    if (!fs::exists(BASE_TMP_DIR_PATH)) {
        fs::create_directory(BASE_TMP_DIR_PATH);
    }
    if (!fs::exists(TMP_DIR_PATH)) {
        fs::create_directory(TMP_DIR_PATH);
    }
    template_count = 0;
    return FBR_RET_CODE_OK;
}

int FBRAddBaseTemplate(const char * content)
{
    if (content == nullptr) {
        return FBR_RET_CODE_INVALID_TEMPLATE;
    }

    for (int i = 0; i < sizeof(content); i++) {
        char c = content[i];
        if (!(std::isdigit(c) || std::isspace(c) || c == '\n' || c == '\0')) {
            return FBR_RET_CODE_INVALID_TEMPLATE;
        }
    }
    std::string filename2 = (TMP_DIR_PATH / ("base" + std::to_string(template_count) + ".xyt")).string();
    if(Fbr::WriteFile(filename2, content) != 0) {
        return FBR_RET_CODE_FILESYSTEM_ERROR;
    }
    base_templates[template_count++] = filename2;
    return FBR_RET_CODE_OK;
}

int FBRClearBaseTemplates()
{
  std::fill_n(base_templates, template_count, "");
  return FBR_RET_CODE_OK;
}

int FBRSetMatchCallback(FBR_MATCH_CB cb)
{
    callback = cb;
    return FBR_RET_CODE_OK;
}

int FBRCheckDevice()
{
    std::ostringstream oss;
    oss << TMP_DIR_PATH / "check.bmp";
    std::string bitmap = oss.str();
    try
    {
        Scanner scanner(bitmap, callback);
    }
    catch (const ScannerException& e)
    {
        return 1;
    }
    return FBR_RET_CODE_OK;
}

int FBRExtractFingerprintTemplate(char*& content)
{
    std::string name = "match";

    std::ostringstream oss;
    oss << TMP_DIR_PATH / (name + ".bmp");
    std::string bitmap = oss.str();

    try
    {
        Scanner scanner(bitmap, callback);
        scanner.ScanImage();
    }
    catch (const ScannerException& e)
    {
        return 1;
    }

    try {
        Cwsq cwsq(bitmap);
        cwsq.Execute();
        Mindtct mindtct((TMP_DIR_PATH / name).string());
        mindtct.Execute();
    } catch (CwsqException &e) {
        LOG(e.what());
        return FBR_RET_CODE_CWSQ_ERROR;
    } catch (MindtctException &e) {
        LOG(e.what());
        return FBR_RET_CODE_MINDTCT_ERROR;
    }

    std::string filename = (TMP_DIR_PATH / (name + ".xyt")).string();
    std::ifstream file(filename);

    if (!file.is_open()) {
        LOG("Error opening file " + filename);
        return FBR_RET_CODE_FILESYSTEM_ERROR;
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    content = new char[(int)fileSize + 1];

    file.read(content, fileSize);
    content[fileSize] = '\0';

    file.close();
    return FBR_RET_CODE_OK;
}

int FBRValidateLiveFingerprint(int& score)
{
    fs::path match_TMP_DIR_PATH = TMP_DIR_PATH / "match.xyt";
    int counter = 0;
    for (int i = 0; i < template_count; i++) {
        if (base_templates[i].empty()) {
            continue;
        }
        counter++;
    }
    if (counter == 0) {
        return FBR_RET_CODE_BASE_TEMPLATE_NOT_SET;
    }

    char * matchTemplate = nullptr;
    int ret = FBRExtractFingerprintTemplate(matchTemplate);
    if (ret != FBR_RET_CODE_OK) {
        return ret;
    }
    try {
      Bozorth bozorth(match_TMP_DIR_PATH.string(), base_templates);
      bozorth.Execute(score);
    } catch (BozorthException &e) {
      LOG(e.what());
      return FBR_RET_CODE_BOZORTH_ERROR;
    }
    return FBR_RET_CODE_OK;
}

int FBRTerminate()
{
	fs::remove_all(TMP_DIR_PATH);
    return FBR_RET_CODE_OK;
}

char * FBRGetLastError()
{
    return const_cast<char*>(last_error.c_str());
}