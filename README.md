
# URLShortener

A lightweight and efficient URL shortening service written in C, using `libmicrohttpd` for the HTTP server, SQLite for data persistence, and a custom hashing algorithm to generate unique short codes.

> ⚠️ **Project Status**: This project is currently under active development. Core functionality is working, but features and architecture may change. Contributions and feedback are welcome.

## Features

* **Shorten URLs** via a simple `POST /add` request
* **Resolve short codes** using a `GET /resolve/<short_code>` endpoint
* **Efficient routing** and error handling
* **SQLite database** for persistent storage
* **Written in pure C** for performance and control
* **Modular architecture** for clean code separation

## 🛠 Technologies Used

* C99
* [libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/)
* SQLite3

## Build Instructions

### Prerequisites

* `gcc` or `clang`
* `make`
* `libmicrohttpd`
* `sqlite3`

### Build

```bash
make
```

### Run

```bash
./urlshortener
```

The server runs on `localhost:8888`.

## API Usage

### Add URL (POST)

**Endpoint:** `/add`
**Content-Type:** `application/x-www-form-urlencoded`
**Body:**

```bash
url=https://example.com
```

**Responses:**

* `200 OK` — URL added successfully
* `409 Conflict` — Short code already exists
* `400/500` — Errors during request or processing

### Resolve URL (GET)

**Endpoint:** `/resolve/<short_code>`
**Response:** Returns resolved URL or error message

## Project Structure

```
.
├── core.c                # Application logic
├── db.c                  # SQLite interactions
├── hashing.c             # Hash generation
├── http_get_request.c    # GET request handling
├── http_post_request.c   # POST request handling
├── http_core_handling.c  # HTTP setup and routing
├── main.c                # Entry point
└── include/              # Header files
```

## Frontend

A frontend interface for URLShortener is under development and will be added to the [GitHub repository](https://github.com/n4tas/URLShortener) soon.

## License

This project is licensed under the MIT License.

-------
