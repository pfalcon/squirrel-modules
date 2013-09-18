import("socket")

DEFLEN = 4096

class chainstream {

    streams = null;

    constructor(...) {
        streams = vargv
    }
}

class HTTPResponse {

    sock = null
    status = null
    headers = null

    constructor(sock) {
        this.sock = sock
    }

    function read_headers() {
        headers = {}
        local l
        status = sock.readline()
        while ((l = sock.readline()) != "\r\n") {
            l = l.slice(0, -2)
            l = split(l, ":")
            l[1] = strip(l[1])
            headers[l[0]] = l[1]
        }
    }

    function read(len = DEFLEN) {
        if (headers == null)
            read_headers()
        return sock.read(len)
    }
}

class HTTPClient {

    server = null;
    s = null;

    constructor(server) {
        this.server = server
    }

    function request(method, uri) {
        local s = socket.Socket(server, 80)
        s.write(method + " " + uri + " HTTP/1.0\r\n\r\n")
        return HTTPResponse(s)
    }
}

h = HTTPClient([127, 0, 0, 1])
r = h.request("GET", "/")
print(r.read(50))
print(r.read())
print("------------\n")
print("*Status*:" + r.status)
foreach (k, v in r.headers)
    print(k + "=" + v + "\n")
