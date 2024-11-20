#include "Client.h"
#include <iostream>
#include <string>

class Transport{

public:

    Transport() : address(DEFAULT_PORT, DEFAULT_HOST) {}
    void Run(){
        
        while (true) {
            Client client(address);                 // Client object responsible for network communication
            client.ShowCommands();          // Show all available commands
            std::cin >> command;

            if (command == 0) {
                std::cout << "Exit..." << std::endl;
                // Send EXIT command to the server
                Protocol::Message exit_msg;
                exit_msg.command = Protocol::EXIT;  
                client.SendRequest(exit_msg.command, exit_msg.data);
                break;  
            }
            // Prepare the message based on user input
            Protocol::Message request = client.HandleCommand(command);

            try {
                client.SendRequest(request.command, request.data);      // Send the prepared request to the server
                std::string response = client.ReceiveResponse();        // Receive the response from the server

                client.ProcessServerResponse(command, response);

            } catch (const std::exception& e) {
                std::cerr << "Error sending/receiving data: " << e.what() << std::endl;
            }
        }
    }
private:
    Address address;    // Default address of the server
    int command = -1;

};