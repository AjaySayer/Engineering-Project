import http.server
import socketserver
import threading
import os
import sys

# Server configurations
PORT = 8000
DIRECTORY = "."

# Define the HTTP request handler
Handler = http.server.SimpleHTTPRequestHandler

# Function to start the server
def start_server():
    os.chdir(DIRECTORY)
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        print(f"Serving HTTP on port {PORT}")
        httpd.serve_forever()

# Function to stop the server
def stop_server(server_thread):
    print("Stopping the server...")
    if server_thread.is_alive():
        # Stopping the server
        os._exit(0)  # This will kill the process completely, stopping the server

# Function to display the menu
def show_menu():
    print("\nSimple Python HTTP Server")
    print("1. Start server")
    print("2. Stop server")
    print("3. Exit")
    print("Choose an option:")

# Main loop
if __name__ == "__main__":
    server_thread = None

    while True:
        show_menu()
        choice = input()

        if choice == "1":
            if server_thread and server_thread.is_alive():
                print("Server is already running.")
            else:
                # Start the server in a separate thread
                server_thread = threading.Thread(target=start_server)
                server_thread.daemon = True  # Allows program to exit even if thread is running
                server_thread.start()
        elif choice == "2":
            if server_thread and server_thread.is_alive():
                stop_server(server_thread)
            else:
                print("Server is not running.")
        elif choice == "3":
            print("Exiting...")
            if server_thread and server_thread.is_alive():
                stop_server(server_thread)
            sys.exit(0)
        else:
            print("Invalid option. Please choose 1, 2, or 3.")
