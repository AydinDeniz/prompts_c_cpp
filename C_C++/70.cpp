#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <map>
#include <queue>
#include <asio.hpp>

using asio::ip::tcp;

class ChatServer {
public:
    ChatServer(asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

private:
    void start_accept() {
        auto new_session = std::make_shared<Session>(acceptor_.get_io_context(), clients_);
        acceptor_.async_accept(new_session->socket(),
            [this, new_session](const asio::error_code& error) {
                if (!error) {
                    new_session->start();
                }
                start_accept();
            });
    }

    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(asio::io_context& io_context, std::map<tcp::socket, std::shared_ptr<Session>>& clients)
            : socket_(io_context), clients_(clients) {}

        tcp::socket& socket() { return socket_; }

        void start() {
            clients_.emplace(socket_, shared_from_this());
            do_read();
        }

        void deliver(const std::string& message) {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(message);
            if (!write_in_progress) {
                do_write();
            }
        }

    private:
        void do_read() {
            auto self(shared_from_this());
            asio::async_read_until(socket_, asio::dynamic_buffer(input_buffer_), "\n",
                [this, self](std::error_code ec, std::size_t length) {
                    if (!ec) {
                        std::string message = std::string(input_buffer_.substr(0, length));
                        input_buffer_.erase(0, length);
                        broadcast(message, self.get());
                        do_read();
                    } else {
                        clients_.erase(socket_);
                    }
                });
        }

        void do_write() {
            auto self(shared_from_this());
            asio::async_write(socket_,
                asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                [this, self](std::error_code ec, std::size_t) {
                    if (!ec) {
                        write_msgs_.pop_front();
                        if (!write_msgs_.empty()) {
                            do_write();
                        }
                    } else {
                        clients_.erase(socket_);
                    }
                });
        }

        void broadcast(const std::string& message, Session* origin) {
            for (auto& [sock, client] : clients_) {
                if (client.get() != origin) {
                    client->deliver(message);
                }
            }
        }

        tcp::socket socket_;
        std::string input_buffer_;
        std::deque<std::string> write_msgs_;
        std::map<tcp::socket, std::shared_ptr<Session>>& clients_;
    };

    tcp::acceptor acceptor_;
    std::map<tcp::socket, std::shared_ptr<Session>> clients_;
};

class ChatClient {
public:
    ChatClient(asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
        : io_context_(io_context), socket_(io_context) {
        do_connect(endpoints);
    }

    void send(const std::string& msg) {
        asio::post(io_context_,
            [this, msg]() {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(msg);
                if (!write_in_progress) {
                    do_write();
                }
            });
    }

    void close() {
        asio::post(io_context_, [this]() { socket_.close(); });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints) {
        asio::async_connect(socket_, endpoints,
            [this](std::error_code ec, tcp::endpoint) {
                if (!ec) {
                    do_read();
                }
            });
    }

    void do_read() {
        asio::async_read_until(socket_, asio::dynamic_buffer(input_buffer_), "\n",
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::cout << input_buffer_.substr(0, length);
                    input_buffer_.erase(0, length);
                    do_read();
                } else {
                    socket_.close();
                }
            });
    }

    void do_write() {
        asio::async_write(socket_,
            asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
            [this](std::error_code ec, std::size_t) {
                if (!ec) {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty()) {
                        do_write();
                    }
                } else {
                    socket_.close();
                }
            });
    }

    asio::io_context& io_context_;
    tcp::socket socket_;
    std::string input_buffer_;
    std::deque<std::string> write_msgs_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            std::cerr << "Usage: chat <host> <port>\n";
            return 1;
        }

        asio::io_context io_context;
        
        if (std::string(argv[1]) == "server") {
            short port = std::atoi(argv[2]);
            ChatServer server(io_context, port);
            io_context.run();
        } 
        else { // client
            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(argv[1], argv[2]);
            ChatClient client(io_context, endpoints);

            std::thread t([&io_context]() { io_context.run(); });

            char line[512];
            while (std::cin.getline(line, 512)) {
                client.send(line);
                client.send("\n");
            }
            client.close();
            t.join();
        }
    } 
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}