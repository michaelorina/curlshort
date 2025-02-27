# Curlshort - A Lightweight URL Shortener in C  

## **Overview**  
Curlshort is a production-level URL shortener built in C. It features:  
- A simple **HTTP server** to handle URL shortening and redirection.  
- **Base62 encoding** for generating short URLs.  
- **SQLite3 integration** for storing and retrieving URLs.  
- **Memory caching** for fast lookups.  
- **Multi-threading** for handling concurrent requests.  

## **Installation**  

### **Prerequisites**  
Ensure you have the following installed:  
- `gcc` (or `clang`)  
- `sqlite3`  
- `curl` (for testing API requests)  

### **Build & Run**  
```sh
make
./build/curlshort
```
### **Usage**
## Shorten a URL
```sh
curl -X POST -d "https://www.example.com" http://localhost:8080
```
## Response:
```sh
Generated short URL: 1
```
### **Future Improvements**
- Implement Redis for caching
- Add analytics (click count, expiry)
- Dockerize the application
