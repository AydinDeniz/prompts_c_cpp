#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <asio.hpp>

using asio::ip::tcp;

std::string root_directory = ".";

std::unordered_map<std::string, std::string> mime_types = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".gif", "image/gif"},
    // Add more types as needed
};

std::string get_mime_type(const std::string& path) {
    std::string extension = std::filesystem::path(path).extension().string();
    if (mime_types.find(extension) != mime_types.end()) {
        return mime_types[extension];
    }
    return "application/octet-stream";
}

void handle_client(tcp::socket socket) {
    try {
        std::array<char, 1024> buffer;
        asio::error_code error;

        size_t length = socket.read_some(asio::buffer(buffer), error);
        if (error) throw asio::system_error(error);

        std::istringstream request_stream(std::string(buffer.data(), length));
        std::string method;
        std::string uri;
        std::string version;

        request_stream >> method >> uri >> version;

        std::string filepath = root_directory + uri;
        if (filepath.back() == '/') {
            filepath += "index.html";
        }

        std::ifstream file(filepath.c_str(), std::ios::binary);
        std::ostringstream response;

        if (file) {
            file.seekg(0, std::ios::end);
            std::size_t file_size = file.tellg();
            file.seekg(0, std::ios::beg);

            response << "HTTP/1.1 200 OK\r\n";
            response << "Content-Type: " << get_mime_type(filepath) << "\r\n";
            response << "Content-Length: " << file_size << "\r\n";
            response << "Connection: close\r\n\r\n";

            response << file.rdbuf();
        } else {
            std::string not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
            asio::write(socket, asio::buffer(not_found), error);
            if (error) throw asio::system_error(error);
            return;
        }

        asio::write(socket, asio::buffer(response.str()), error);
        if (error) throw asio::system_error(error);

    } catch (std::exception& e) {
        std::cerr << "Exception in handle_client: " << e.what() << std::endl;
    }
}

int main() {
    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        std::cout << "Server running on port 8080\n";

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::thread(handle_client, std::move(socket)).detach();
        }

    } catch (std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    }

    return 0;
}