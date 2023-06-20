#include <crow.h>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

std::map<std::string, std::string> parseBody(const std::string& body)
{
    std::map<std::string, std::string> parsedBody;
    std::istringstream ss(body);
    std::string token;

    while(std::getline(ss, token, '&')) {
        auto equalPos = token.find('=');
        if (equalPos != std::string::npos) {
            std::string key = token.substr(0, equalPos);
            std::string value = token.substr(equalPos+1);
            parsedBody[key] = value;
        }
    }

    return parsedBody;
}

std::string loadHtmlFile(const std::string& filepath)
{
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        return "Error loading file: " + filepath;
    }
    std::stringstream sstr;
    sstr << ifs.rdbuf();
    return sstr.str();
}

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]{
        return loadHtmlFile("/home/anav/Documents/QT PROJECTS/CrowDemo/form.html");
    });

    CROW_ROUTE(app, "/submit")
        .methods("POST"_method)
        ([](const crow::request& req){
            auto parsedBody = parseBody(req.body);
            std::string name = parsedBody.count("name") ? parsedBody["name"] : "[none]";
            std::string age = parsedBody.count("age") ? parsedBody["age"] : "[none]";

            std::ostringstream os;
            os << "Received POST: Name=" << name << ", Age=" << age;

            return crow::response(200, os.str());
        });

    app.port(8080).multithreaded().run();

    return 0;
}
