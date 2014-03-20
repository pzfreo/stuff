import httplib2
from urllib import urlencode
import json
from pprint import pprint
import mosquitto_auth
import base64

introspect_url= "http://localhost:8283/introspect"
intro_username="admin"
intro_password="admin"

def plugin_init(opts):
    print 'plugin_init'
    pprint(opts)

def plugin_cleanup():
    print 'plugin_cleanup'

def unpwd_check(username, password):
    print "unpwd check: " + username + " " + password
    if (username=="r" and password=="r"):
        return True
        
    if (username=="admin" and password=="admin"):
        return True
    valid, scopes = call_token_introspection(username, password)
    return valid

def acl_check(clientid, username, topic, access):
    print "acl_check"
    print "client "+clientid
    print "topic "+ topic
    if (username=="admin"):
        return True
    if (username==""):
        return False
    # first check if this is just aimed at the client
    if (mosquitto_auth.topic_matches_sub('/c/#', topic) and access == mosquitto_auth.MOSQ_ACL_WRITE):
       return True # anyone can write to a client.
    if ((topic == '/c/'+clientid) and access == mosquitto_auth.MOSQ_ACL_READ):
       return True # this client can read messages destined for itself
    
    if (mosquitto_auth.topic_matches_sub('/c/#', topic) and access == mosquitto_auth.MOSQ_ACL_READ):
       print "unauthorized attempt to subscribe to another client"
       return False # otherwise can't read
       
    # refresh user can post to /refresh and nothing else
    if (username=="r"):
        if (topic=="/r" and access==mosquitto_auth.MOSQ_ACL_WRITE):
            print "refresh user posting to /r"
            return True
        else:
            return False
            
    valid, scopes = call_token_introspection(username, "")
    print "valid", valid
    print "scopes", scopes
    print clientid, username, topic, access
    if (valid):
        write = (access == mosquitto_auth.MOSQ_ACL_WRITE);
        for  scope in scopes:
            rw = (scope['rw']).encode('ascii').lower()  
            print rw
            if (rw=="rw" or rw=="wr" or (write and rw=="w")  or ((not write) and rw=="r")):
                print scope['topic']
                if (mosquitto_auth.topic_matches_sub(scope['topic'], topic)):
                    return True
    return False
        

def security_init(opts, reload):
    print 'security_init', reload
    pprint(opts)
    return True

def security_cleanup(reload):
    print 'security_cleanup', reload
    return True


def call_token_introspection(username, password):
    print "================"
    print "calling introspection with: "+username
    h = httplib2.Http( disable_ssl_certificate_validation=True)
    #h.add_credentials(intro_username, intro_password)

    base64string = base64.b64encode(intro_username+":"+intro_password)
    authheader =  "Basic " + base64string
    data = dict(token=username, token_type_hint="bearer")
    resp, content = h.request(introspect_url,
       "POST", body=urlencode(data),
       headers={'content-type':'application/x-www-form-urlencoded', 'Authorization':authheader} )
    print resp            
    response=json.loads(content)
    if ('error' in response):
        return False, ""
    if response['active']!='true':
        print 'Inactive token'
        return False, ""
    else:
        scopes = json.loads(base64.b64decode(response['scope']))
        return True, scopes
        


             