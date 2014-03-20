import webbrowser
import base64
from urllib import urlencode
import SimpleHTTPServer
import SocketServer
import threading
import httplib2
import json
import sys
import time



clientid = "oefnUnEFx9tyLr9MwHykI8x0Vqga"
clientsecret = "qtElvfENMRf72pyQEKrIRToZoMUa"

scope = '[{"rw":"rw","topic":"/pzf/#"}]'
authorizeurl="http://localhost:9763/oauth2/authorize"
token_url ="http://localhost:9763/oauth2/token"

print base64.b64encode(scope)

data = dict(client_id=clientid, scope=base64.b64encode(scope), redirect_uri="http://localhost:8080/test", response_type="code")
webbrowser.open_new(authorizeurl+"?"+urlencode(data))


class MyRequestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def do_GET(self):
        print self.path
        code = self.path[11:]
        print code
        h = httplib2.Http( disable_ssl_certificate_validation=True)
        data = dict(grant_type="authorization_code", code=code, client_id=clientid, client_secret=clientsecret,
            redirect_uri="http://localhost:8080/test");
        resp, content = h.request(token_url, "POST", body=urlencode(data),
        headers={'content-type':'application/x-www-form-urlencoded'} )
        print resp
        response=json.loads(content)
        print response
        self.send_response(200)
        self.send_header('Content-type','text/plain')
        self.end_headers()
        self.wfile.write("Please cut and paste the following code to your Arduino EEPROM writer:\n")
        self.wfile.write("char refresh[] = \""+response['refresh_token']+"\";\n")
        self.wfile.write("char bearer[] = \""+response['access_token']+"\";\n")

        return 
        
PORT = 8080

# class ListeningThread(threading.Thread):
#      def __init__(self):
#          threading.Thread.__init__(self)
#     
#      def run(self):
#          Handler = MyRequestHandler
#          httpd = SocketServer.TCPServer(("", PORT), Handler)
#          server = httpd
#          httpd.serve_forever()
# 
# thread1 = ListeningThread()
# thread1.start() 

Handler = MyRequestHandler
httpd = SocketServer.TCPServer(("", PORT), Handler)
httpd.serve_forever()
print "serving at port", PORT

time.sleep(20);
httpd.shutdown();






