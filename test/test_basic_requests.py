import http.client

GREEN = '\033[1;92m'
RED = '\033[1;91m'
RESET = '\033[0m'

def test_request(host, port, http_method, uri, payload, expected_status, expected_body):
	test_name = f"Test {http_method} http://{host}:{port}{uri}"

	conn = http.client.HTTPConnection(host, port)

	request_headers = {
		"Accept": "*/*",
		"User-Agent": "Python Test Script",
		"content-type": "text/plain" 
	}

	conn.request(http_method, uri, payload, request_headers)
	response = conn.getresponse()

	status = response.status
	headers = response.getheaders()
	body = response.read().decode("utf-8")

	if status == expected_status and (len(expected_body) == 0 or body == expected_body):
		print(f"{test_name} {GREEN}OK! :){RESET}")
	else:
		print(f"{test_name} {RED}FAIL! :({RESET}")
		print("\tExpected values:")
		print(f"\t\t- Status: {expected_status}")
		if (len(expected_body) > 0):
			print(f"\t\t- Body: {expected_body}")
		print("\tResponse values:")
		print(f"\t\t- Status: {status}")
		if (len(expected_body) > 0):
			print(f"\t\t- Body: {body}")
		print("")

test_request("localhost", 8080, "GET", "/hello-world.html", "", 200, "Hello World!")
test_request("localhost", 8080, "GET", "/index.html", "", 200, "")

test_request("localhost", 8081, "POST", "/cgi-bin/echo.py", "Payload", 413, "")
test_request("localhost", 8080, "POST", "/cgi-bin/echo.py", "Payload", 200, "Payload")

test_request("localhost", 8080, "POST", "/cgi-bin/exception.py", "", 500, "")
