/*
 *
 *  Created on: Mar 29, 2012
 *      Author: tony
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdint.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include "zht_util.h"
#include "novoht.h"

using namespace std;
#define MAXEVENTS 64
#define PORT_FOR_REPLICA 50009
NoVoHT *pmap; //move to main().
map<string, string> hmap; //for pure non-persistency

char* LISTEN_PORT; // server listen port

const int MAX_NUM_REPLICA = 3;

struct HostEntity Replicas[MAX_NUM_REPLICA];

bool TCP; // for switch between TCP and UDP

BdRecvBase *pbrb;

//For queueing request
class DataEvent {
public:
	int FD;
	void* buffer;
	size_t bufsize;
	sockaddr_in fromAddr;

	DataEvent(int fd, void* buf, size_t bufsize, sockaddr_in addr) {
		this->FD = fd;
		int len = strlen((const char*) buf);
		this->buffer = malloc((len + 1) * sizeof(char));
		memcpy(this->buffer, buf, len + 1);

		this->bufsize = bufsize;
		this->fromAddr = addr;
	}
	;
	~DataEvent() {
		//free(this->buffer);
	}
};

struct threadArg {
	NoVoHT* novoht;
	queue<DataEvent>* myQueue;
};

static int make_socket_non_blocking(int sfd) {
	int flags, s;

	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1) {
		perror("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl(sfd, F_SETFL, flags);
	if (s == -1) {
		perror("fcntl");
		return -1;
	}
	return 0;
}

static int create_and_bind(char *port) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; // Return IPv4 and IPv6 choices
	hints.ai_socktype = SOCK_STREAM; // We want a TCP socket
	hints.ai_flags = AI_PASSIVE; // All interfaces

	s = getaddrinfo(NULL, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0) {
			// We managed to bind successfully!
			break;
		}

		close(sfd);
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		return -1;
	}

	freeaddrinfo(result);

	return sfd;
}

//parse buff and handle it.
int handleRequest(int sock, void*buff) {

	return 0;
}
/*
 int32_t HB_insert(NoVoHT *map, Package &package) {
 //int opt = package.operation();//opt not be used?
 string package_str = package.SerializeAsString();
 //int ret = db.set(package.virtualpath(), package_str); //virtualpath as key
 //	cout<<"Insert to pmap..."<<endl;
 string key = package.virtualpath();
 //	cout<<"key:"<<key<<endl;
 string value = package_str;
 //	cout<<"value:"<<value<<endl;
 //	cout<<"Insert: k-v ready. put..."<<endl;
 int ret = map->put(key, value);
 //	cout << "end inserting, ret = " << ret << endl;

 if (ret != 0) {
 return -2;
 }

 // cout << "String insted: " << package_str << endl;

 else
 return 0;
 }

 string HB_lookup(NoVoHT *map, Package &package) {
 string value;
 //	cout << "lookup in HB_lookup" << endl;
 string key = package.virtualpath();
 //	cout << "key:" << key << endl;
 string *strP = map->get(key); //problem
 //	cout << "lookup end." << endl;

 if (strP == NULL) {
 cout << "lookup find nothing." << endl;
 string nullString = "Empty";
 return nullString;
 }
 return *strP;
 }



 int32_t HB_remove(NoVoHT *map, Package &package) {
 string key = package.virtualpath();
 int ret = map->remove(key); // return 0 means correct.
 if (ret != 0) {
 cout << "DB Error: fail to remove :ret= " << ret << endl;
 return -2;
 } else
 return 0; //succeed.
 }
 */

int32_t HB_insert(NoVoHT *map, Package &package) {
	//int opt = package.operation();//opt not be used?
	string value = package.SerializeAsString();

	//int ret = db.set(package.virtualpath(), package_str); //virtualpath as key
//	cout << "Insert to pmap...value = " << value << endl;
	string key = package.virtualpath();

//      cout<<"key:"<<key<<endl;

//      cout<<"value:"<<value<<endl;
//      cout<<"Insert: k-v ready. put..."<<endl;
	int ret = map->put(key, value);
//      cout << "end inserting, ret = " << ret << endl;

	if (ret != 0) {
		cerr << "insert error: ret = " << ret << endl;
		return -3;
	}
	/*
	 cout << "String insted: " << package_str << endl;
	 */
	else
		return 0;
}

int32_t HB_append(NoVoHT *map, Package &package) {
	string value = package.SerializeAsString();
//      cout << "Insert to pmap...value = " << value << endl;
	string key = package.virtualpath();
//      cout<<"key:"<<key<<endl;
//      cout<<"value:"<<value<<endl;
	int ret = map->append(key, value);
//      cout << "end inserting, ret = " << ret << endl;
	if (ret != 0) {
		cerr << "Append error: ret = " << ret << endl;
		return -4;
	} else
		return 0;
}

string HB_lookup(NoVoHT *map, Package &package) {
//      string value;
//      cout << "lookup in HB_lookup" << endl;
	string key = package.virtualpath();
//	cout << "key:" << key << endl;
	// string *strP = map->get(key); //problem
	string *result = map->get(key);

//	cout << "lookup result = " << (*result) << endl;

	if (result == NULL) {
		cout << "lookup find nothing." << endl;
		string nullString = "Empty";
		return nullString;
	} else {
		string retStr((*result));
		return retStr;
	}

}

int32_t HB_remove(NoVoHT *map, Package &package) {
	string key = package.virtualpath();
	int ret = map->remove(key); // return 0 means correct.
	if (ret != 0) {
		cerr << "DB Error: fail to remove :ret= " << ret << endl;
		return -2;
	} else
		return 0; //succeed.
}

/*bool eqstr(char *s1, char *s2) {
 return strcmp(s1, s2) == 0;
 }*/

int32_t HB_compare_and_swap(NoVoHT *map, Package &package) {

	/*get Package stored by lookup*/
	string lresult = HB_lookup(map, package);
	Package lpackage;
	lpackage.ParseFromString(lresult);

	/*get seen_value stored*/
	string seen_value_stored = lpackage.realfullpath();

	string seen_value_pass_in = package.realfullpath();

	/*they are equivalent*/
	if (!seen_value_stored.compare(seen_value_pass_in)) {

		package.set_realfullpath(package.newfullpath());
		return HB_insert(map, package);

	} else {
		return -5;
	}
}

int32_t HB_compare_and_swap_(NoVoHT *map, Package &package) {
	string curStr = HB_lookup(map, package);
	string seenStr = package.realfullpath();
	if (!curStr.compare(seenStr)) {
		package.set_realfullpath(package.newfullpath());
		return HB_insert(map, package);
	} else {
		return -5;
	}
}

struct charscmp: public std::binary_function<const char*, const char*, bool> {
	bool operator()(const char* s1, const char* s2) const {
		return strcmp(s1, s2) < 0;
	}
};

typedef map<const char*, const char*, charscmp> MY_MAP;
typedef pair<const char*, const char*> MY_PAIR;
static MY_MAP chmap;

int32_t HB_insert_cstr(MY_MAP &chmap, Package &package) {

	string package_str = package.SerializeAsString();

	char* value = (char*) calloc(package_str.length(), sizeof(char));
	strcpy(value, package_str.c_str());

	char* key = (char*) calloc(package.virtualpath().length(), sizeof(char));
	strcpy(key, package.virtualpath().c_str());

//	cout <<"after scrcpy, key = "<<key<<", key length = "<< strlen(key) <<endl;

//      pair<map<char*, char*>::iterator, bool> ret;
//      ret = chmap.insert(pair<char*, char*>(key, value));
//      free(key);
//      free(value);

	pair<MY_MAP::iterator, bool> ret;
	ret = chmap.insert(MY_PAIR(key, value));

	MY_MAP::iterator it;
	cout << "mymap.size() is " << (int) chmap.size() << endl;
	cout << "mymap contains:\n";
	for (it = chmap.begin(); it != chmap.end(); it++)
		cout << (*it).first << " => " << (*it).second << endl;
	cout << "########" << endl;

	if (ret.second == false) {
		cout
				<< "HB_insert_cstr: insert failed, return -3, element exists, key = "
				<< key << ", value = " << value << endl;

		cout << "######## done ########" << endl;
		free(key);
		free(value);
		return -3;
	} else {
		cout << " HB_insert_cstr: insert succeeded. key = " << key
				<< ", value = " << chmap.find(key)->second << endl;

		cout << "######## done ########" << endl;
		free(key);
		free(value);
		return 0;
	}

}

int32_t HB_insert_cstr_(map<char*, char*> &chmap, Package &package) {

	string package_str = package.SerializeAsString();

	char* value = (char*) malloc(package_str.length() * sizeof(char));
	strcpy(value, package_str.c_str());

	char* key = (char*) malloc(package.virtualpath().length() * sizeof(char));
//	cout << "package.virtualpath().c_str()="<<package.virtualpath().c_str()<<endl;
	strcpy(key, package.virtualpath().c_str());

//	cout <<"after scrcpy, key = "<<key<<", key length = "<< strlen(key) <<endl;

//      pair<map<char*, char*>::iterator, bool> ret;
//      ret = chmap.insert(pair<char*, char*>(key, value));
//      free(key);
//      free(value);
	if (chmap.insert(pair<char*, char*>(key, value)).second == false) {
		cout
				<< "HB_insert_cstr: insert failed, return -3, element exists, key = "
				<< key << ", value = " << value << endl;
		free(key);
		free(value);
		return -3;
	} else {
		cout << " HB_insert_cstr: insert succeeded. key = " << key
				<< ", value = " << chmap.find(key)->second << endl;
		free(key);
		free(value);
		return 0;
	}

}

int32_t HB_insert(map<string, string> &hmap, Package &package) {
	//int opt = package.operation();//opt not be used?
	string package_str = package.SerializeAsString();
	//int ret = db.set(package.virtualpath(), package_str); //virtualpath as key
	//      cout<<"Insert to pmap..."<<endl;
	string key = package.virtualpath();
	//      cout<<"key:"<<key<<endl;
	string value = package_str;
	//      cout<<"value:"<<value<<endl;
	//      cout<<"Insert: k-v ready. put..."<<endl;

	//pair<map<string,string>::iterator,bool> ret;
	pair<map<string, string>::iterator, bool> ret;
	ret = hmap.insert(pair<string, string>(key, value));

	if (ret.second == false) {
		return -3;
	} else
		return 0;
}

string HB_lookup(map<string, string> &hmap, Package &package) {
	string value;
//              cout << "lookup in HB_lookup" << endl;
	string key = package.virtualpath();
//              cout << "key:" << key << endl;
	map<string, string>::iterator it;
	it = hmap.find(key);
	if (it == hmap.end()) {
		string nullString = "Empty";
		return nullString;
	}
	return (*it).second;
}

int32_t HB_remove(map<string, string> &hmap, Package &package) {
	unsigned int r = hmap.erase(package.virtualpath());

	if (r == 0) {

		cout << "Remove nothing, no match found, key=" << package.virtualpath()
				<< endl;
		return -1;
	}
	return 0;

}

struct threaddata {
	int socket;
	NoVoHT *p_pmap;
	char receivedData[]; //or char* something?
};

int turn_off;
vector<struct HostEntity> hostList;
int nHost;
static int server_sock = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int numthreads = 0;

/*
 int socket_replica_old(Package package, struct HostEntity destination) {
 string str = package.SerializeAsString();

 int to_sock = socket(PF_INET, SOCK_STREAM, 0); //try change here.................................................

 //socket(nmspace,style,protocol), originally be socket(AF_INET, SOCK_STREAM, 0)

 struct sockaddr_in dest, recv_addr;
 memset(&dest, 0, sizeof(struct sockaddr_in));
 struct hostent * hinfo = gethostbyname(destination.host.c_str());
 if (hinfo == NULL)
 printf("getbyname failed!\n");
 dest.sin_family = PF_INET; //storing the server info in sockaddr_in structure
 dest.sin_addr = *(struct in_addr *) (hinfo->h_addr); //set destination IP number
 dest.sin_port = htons(destination.port);

 int ret_con = connect(to_sock, (struct sockaddr *) &dest, sizeof(sockaddr));
 if (ret_con < 0) {
 cerr << "socket_replica: error on connect(): " << strerror(errno)
 << endl;
 return -1;
 }

 int optval = 1;

 if (setsockopt(to_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval)
 < 0)
 cerr << "replica: reuse failed." << endl;
 if (to_sock < 0) {
 cerr << "socket_replica: error on socket(): " << strerror(errno)
 << endl;
 return -1;
 }
 int ret_snd = send(to_sock, (const void*) str.c_str(), str.size(), 0); // may try other flags......................
 if (ret_snd < 0) {
 cerr << "socket_replica: error on socket(): " << strerror(errno)
 << endl;
 return -1;

 }

 void *buff_return = (void*) malloc(sizeof(int32_t));
 int r = d3_svr_recv(to_sock, buff_return, sizeof(int32_t), 0, &recv_addr);
 //connect (int socket, struct sockaddr *addr, size_t length)
 if (r < 0) {
 cerr << "socket_replica: got bad news from relica: " << r << endl;
 }

 close(to_sock);
 }
 */

int makeConnForReplica(struct HostEntity &dest) {
	int sock = 0;
	int index = -1;
//	cout << "makeConnForReplica ..........  1" << endl;

	//	cout<<"str2Sock: dest.sock = "<<dest.sock<<endl;
	if (dest.sock < 0) {
//		cout << "makeConnForReplica ..........  2" << endl;
		//sock = makeClientSocket((char*) dest.host.c_str(), dest.port, true);
		sock = makeClientSocket((char*) dest.host.c_str(), dest.port, TCP);
//		cout << "makeConnForReplica ..........  3" << endl;
		reuseSock(sock);
//		cout << "makeConnForReplica ..........  4" << endl;
		dest.sock = sock;
	}
//	cout << "makeConnForReplica ..........  7" << endl;
	return dest.sock;
}

int socket_replica(Package package, struct HostEntity &destination) {
	package.set_replicano(3);
	string str = package.SerializeAsString();
//	cout << "socket_replica--------1" << endl;
//	cout << "socket_replica--------before makeConnForReplica sock = "<< destination.sock << endl;
	int sock = makeConnForReplica(destination); //reusable sockets creation
//	cout << "socket_replica--------after makeConnForReplica sock = "<< destination.sock << endl;
//	int sock = makeClientSocket("localhost", 50009, true);

//	cout << "socket_replica--------2,  sock = " << sock << endl;

	//	generalSend(destination.host, destination.port, sock, str.c_str(), 1);
//	cout << "socket_replica--------2, sock = " << sock << endl;
	generalSendTCP(sock, str.c_str(), str.size());

//	cout << "socket_replica--------3" << endl;
	void *buff_return = (void*) malloc(sizeof(int32_t));
	//	int r = d3_svr_recv(sock, buff_return, sizeof(int32_t), 0, &recv_addr);
	//	int r = generalReveiveTCP(sock, buff_return, sizeof buff_return, 0);
	int r = 0;
//	cout << "socket_replica--------4" << endl;
	//connect (int socket, struct sockaddr *addr, size_t length)
	if (r < 0) {
		cerr << "socket_replica: got bad news from relica: " << r << endl;
	}

}

int general_replica(Package package, struct HostEntity &destination) {
	package.set_replicano(3);
	string str = package.SerializeAsString();
//      cout << "socket_replica--------1" << endl;
//      cout << "socket_replica--------before makeConnForReplica sock = "<< destination.sock << endl;
	int sock = makeConnForReplica(destination); //reusable sockets creation
//      cout << "socket_replica--------after makeConnForReplica sock = "<< destination.sock << endl;
//      int sock = makeClientSocket("localhost", 50009, true);

//      cout << "socket_replica--------2,  sock = " << sock << endl;

	//      generalSend(destination.host, destination.port, sock, str.c_str(), 1);
//      cout << "socket_replica--------2, sock = " << sock << endl;
//        generalSendTCP(sock, str.c_str());
	generalSendTo(destination.host.c_str(), destination.port, sock, str.c_str(),
			str.size(), TCP);
//      cout << "socket_replica--------3" << endl;
	void *buff_return = (void*) malloc(sizeof(int32_t));
	//      int r = d3_svr_recv(sock, buff_return, sizeof(int32_t), 0, &recv_addr);
	// int r = generalReveiveTCP(sock, buff_return, sizeof buff_return, 0);
	struct sockaddr_in recvAddr;
//		int r =generalReceive(sock, buff_return, sizeof(int32_t), recvAddr, 0, TCP);
	int r = 0;
//      cout << "socket_replica--------4" << endl;
	//connect (int socket, struct sockaddr *addr, size_t length)
	if (r < 0) {
		cerr << "general_replica: got bad news from relica: " << r << endl;
	}
}

bool thread_run = 0;

void dataService(int client_sock, void* buff, size_t bufsize,
		sockaddr_in fromAddr, NoVoHT* pmap) {

//	cout << strlen((char*)buff) << "{" << ((char*)buff) << "}" << endl;

//cout<<"dataService: from port "<<	fromAddr.sin_port<<endl;

	srand(getpid() + clock());
	//srand(kyotocabinet::getpid() + clock());
	//cout << "Current service thread ID = " << pthread_self()<< ", dbService() begin..." << endl;

//	char buff[Env::MAX_MSG_SIZE];
	int32_t operation_status = -99; //
//	sockaddr_in toAddr;
	int r;
	void* buff1;

	Package package;
//	package.ParseFromArray(buff, Env::MAX_MSG_SIZE);
	package.ParseFromArray(buff, bufsize);

	if (!package.opcode().compare("ack")) {

		cout << "ack message" << endl;
		enqueueAck(package);
		return;
	}

	string result;
//	cout << endl << endl << "in dbService: received replicano = "<< package.replicano() << endl;

//	cout << "Server got package size: " << package.ByteSize() << endl;
//	cout <<"Package content: "<< (char*)buff<<endl;

	switch (package.operation()) {
	case 1: //lookup
	{
//		cout << "Lookup..." << endl;
		if (package.virtualpath().empty()) {
//			cerr << "Bad key: nothing to find" << endl;
			operation_status = -1;
		} else {
			//result = HB_lookup(db, package);
//			cout << "Lookup...2" << endl;
//cout<<"Will lookup key: "<< package.virtualpath()<<endl;
//			result = HB_lookup(hmap, package);
			result = HB_lookup(pmap, package);
//			cout << "Lookup...3" << endl;
			//don't really send result back to client now, do it latter.

			if (result.compare("Empty") == 0) {
				operation_status = -2;
			} else {
				operation_status = 0;
			}
		}

		/*
		 * pack the status and lookup-result into one string
		 */
		buff1 = &operation_status;
		char statusBuff[3];
		sprintf(statusBuff, "%03d", operation_status);
		string sAllInOne;
		sAllInOne.append(statusBuff);
		sAllInOne.append(result);

		generalSendBack(client_sock, sAllInOne.c_str(), sAllInOne.size(),
				fromAddr, 0, TCP);
	}
		break;

	case 2: {
		//remove
		//		cout << "Remove..." << endl;
		//cout << "Package:key "<<package.virtualpath()<<endl;
		if (package.virtualpath().empty()) {
			cerr << "Bad key: nothing to remove" << endl;
			operation_status = -1;
		} else {
			//operation_status = HB_remove(db, package);
			//			operation_status = HB_remove(hmap, package);
			operation_status = HB_remove(pmap, package);
			//r = d3_send_data(client_sock, buff1, sizeof(int32_t), 0, &toAddr);
			//r = generalSendBack(client_sock, (const char*) buff1, fromAddr, 0,TCP);
		}

		buff1 = &operation_status;
		if (TCP == true) {
			r = send(client_sock, &operation_status, sizeof(int32_t), 0);
		} else {
			r = sendto(client_sock, &operation_status, sizeof(int32_t), 0,
					(struct sockaddr *) &fromAddr, sizeof(struct sockaddr));
		}

		if (r <= 0) {
			cout
					<< "Remove: Server could not send acknowledgement to client, r = "
					<< r << endl;
		}
		//			cout << "Remove succeeded, return " << operation_status << endl;
		//end remove if-else
	}
		break;
	case 3: {
		//insert
		if (package.virtualpath().empty()) {
			operation_status = -1;
		} else {
			//		cout << "Insert..." << endl;
			//operation_status = HB_insert(db, package);
			operation_status = HB_insert(pmap, package);
//			operation_status = HB_insert_cstr(chmap, package);
			//		operation_status = HB_insert(hmap, package);
			//cout<<"Inserted: key: "<< package.virtualpath()<<endl;
			//		cout << "insert finished, return: " << operation_status << endl;
			//		r = d3_send_data(client_sock, buff1, sizeof(int32_t), 0, &toAddr);

			//		r = generalSendBack(client_sock, (const char*)&operation_status, fromAddr, 0, TCP);
		}

		buff1 = &operation_status;
		if (TCP == true) {
			r = send(client_sock, &operation_status, sizeof(int32_t), 0);
		} else {
			r = sendto(client_sock, &operation_status, sizeof(int32_t), 0,
					(struct sockaddr *) &fromAddr, sizeof(struct sockaddr));
		}

		//cout << "Insert: Server  send acknowledgement to client: sendto r = " <<r<< endl;
		//cout<<"send back status: "<< *(int*)buff1<<endl;
		if (r <= 0) {
			cout
					<< "Insert: Server could not send acknowledgement to client: sendto r = "
					<< r << endl;
		}
	}
		break;
	case 4: {
		if (package.virtualpath().empty()) {
			operation_status = -1;
		} else {
			// cout << "Server: append..." << endl;
			//operation_status = HB_insert(db, package);
			operation_status = HB_append(pmap, package);
			//                      cout << "Server: append ret = "<< operation_status <<endl;
			//cout<<"Inserted: key: "<< package.virtualpath()<<endl;
			//              cout << "insert finished, return: " << operation_status << endl;
		}
		buff1 = &operation_status;
		if (TCP == true) {
			r = send(client_sock, &operation_status, sizeof(int32_t), 0);
		} else {
			r = sendto(client_sock, &operation_status, sizeof(int32_t), 0,
					(struct sockaddr *) &fromAddr, sizeof(struct sockaddr));
		}

		if (r <= 0) {
			cout
					<< "Append: Server could not send acknowledgement to client: sendto r = "
					<< r << endl;
		}

	}
		break;

	case 5: // compare and swap operation
	{
		if (package.virtualpath().empty()) {
			operation_status = -1;
		} else {
			operation_status = HB_compare_and_swap(pmap, package);
		}

		string result = HB_lookup(pmap, package);

		/*
		 * pack the status and lookup-result into one string
		 */
		char statusBuff[3];
		sprintf(statusBuff, "%03d", operation_status);
		string sAllInOne;
		sAllInOne.append(statusBuff);
		sAllInOne.append(result);

		r = generalSendBack(client_sock, sAllInOne.c_str(), sAllInOne.size(),
				fromAddr, 0, TCP);

		if (r <= 0) {
			cout
					<< "compare_and_swap: Server could not send acknowledgement to client: sendto r = "
					<< r << endl;
		}

	}
		break;

	case 99: { //shut the server
//		cout << "Server will be shut shortly." << endl;
		turn_off = 1; //turn off service.
	}
		break;
	default: {
		operation_status = -98; //unrecognized operation

		buff1 = &operation_status;
		if (TCP == true) {
			r = send(client_sock, &operation_status, sizeof(int32_t), 0);
		} else {
			r = sendto(client_sock, &operation_status, sizeof(int32_t), 0,
					(struct sockaddr *) &fromAddr, sizeof(struct sockaddr));
		}
	}
		break;
	} //end switch-case

	buff1 = &operation_status;

//	cout << "Before handle Replication " << endl;
	if (Env::NUM_REPLICAS > 0) { // infinite loop if not limited by replicano, coz it will send the replica to itself infinitely
		if (package.replicano() == 5) {
			if (package.operation() == 3 || package.operation() == 2) {

				int i = Env::NUM_REPLICAS;
				unsigned int n;
				//			package.set_replicano(3);
				while (i > 0) { //change from numReplica to i
					n = myhash((package.virtualpath()).c_str(), nHost) + i;
					n = n % hostList.size();
					struct HostEntity destination = hostList.at(n);
					general_replica(package, Replicas[i - 1]);
					i--;
				}
			}
		}
	}

} //end function

void* dataServiceThread(void* argument) {
	struct threadArg* myArgu = (struct threadArg*) argument;

	while (thread_run) {
		if (!myArgu->myQueue->empty()) {
			DataEvent data = myArgu->myQueue->front();
			dataService(data.FD, data.buffer, data.bufsize, data.fromAddr,
					myArgu->novoht);
			myArgu->myQueue->pop();
		}
	}

}

int Host2Index(const char* hostName) {

	int listSize = hostList.size();
	HostEntity host;
	int i = 0;
	for (i = 0; i < listSize; i++) {
		host = hostList.at(i);

		if (!strcmp(host.host.c_str(), hostName)) {
			break;
		}
	}

	if (i == listSize) {
		return -1;
	}

	return i;

	Replicas[0].host = hostList.at(i).host;
	Replicas[0].port = PORT_FOR_REPLICA;
	Replicas[1].host = hostList.at(i + 1).host;
	Replicas[1].port = PORT_FOR_REPLICA;

}

int main(int argc, char *argv[]) {

	if (argc != 5) { //or 3?
		fprintf(stderr, "Usage: %s [port]\n", argv[0]);
		cout << "argc = " << argc << endl;
		exit(EXIT_FAILURE);
	}

	char* isTCP = argv[4];

	if (!strcmp("TCP", isTCP)) {
		TCP = true;
	} else {
		TCP = false;
	}

	LISTEN_PORT = argv[1];
	string cfgFile(argv[3]);
	string randStr = randomString(5);

	const char* fileName = "";
	pmap = new NoVoHT(fileName, 100000, 10000, 0.7);

	map<string, string> hashMap;
	hmap = hashMap;

	string membershipFile(argv[2]);
	hostList = getMembership(membershipFile);
	nHost = hostList.size();

	Host2Index("localhost");
	if (Env::setconfigvariables(cfgFile) != 0) {
		cout << "Server: Not able to read configuration file." << endl;
		exit(1);
	}

	int myIndex = Host2Index("localhost");
	Replicas[0].host = hostList.at((myIndex + 1) % nHost).host;
	Replicas[0].port = PORT_FOR_REPLICA;
	Replicas[0].sock = -1;

	Replicas[1].host = hostList.at((myIndex + 2) % nHost).host;
	Replicas[1].port = PORT_FOR_REPLICA;
	Replicas[1].sock = -1;

	int listener, s;
	int efd;
	struct epoll_event event;
	struct epoll_event *events;

	listener = makeSvrSocket(atoi(LISTEN_PORT), TCP);
	if (listener == -1)
		abort();

	s = make_socket_non_blocking(listener);
	if (s == -1)
		abort();

	if (TCP == true) {
		s = listen(listener, SOMAXCONN);
		if (s == -1) {
			perror("listen");
			abort();
		}
	}

	reuseSock(listener);

	efd = epoll_create(1); // epoll_create(int size): Nowadays, size is unused

	if (efd == -1) {
		perror("epoll_create");
		abort();
	}

	event.data.fd = listener;
	event.events = EPOLLIN | EPOLLET;
	s = epoll_ctl(efd, EPOLL_CTL_ADD, listener, &event);
	if (s == -1) {
		perror("epoll_ctl");
		abort();
	}

	events = (epoll_event *) calloc(MAXEVENTS, sizeof event);
//	char buf[Env::MAX_MSG_SIZE];

	int epollCounter = 0;

	queue<DataEvent> dataQueue;
	pthread_t idThread;
	thread_run = 1;
	threadArg argu;
	argu.myQueue = &dataQueue;
	argu.novoht = pmap;
	int r = pthread_create(&idThread, NULL, dataServiceThread, (void*) &argu);

	pbrb = new BdRecvFromClient();

	while (1) {
		int n, i;

		n = epoll_wait(efd, events, MAXEVENTS, -1);

		epollCounter++;
//		printf("epoll %d times ", epollCounter);

		for (i = 0; i < n; i++) {
			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
					|| (!(events[i].events & EPOLLIN))) {
				// An error has occured on this fd, or the socket is not ready for reading (why were we notified then?)
				fprintf(stderr, "epoll error\n");
				close(events[i].data.fd);
				continue;
			} else if (listener == events[i].data.fd) { //TCP has new connection:  here UDP should take over
				// We have a notification on the listening socket, which means one or more incoming connections.
				if (TCP == true) {
					while (1) {
						struct sockaddr in_addr;
						socklen_t in_len;
						int infd;
						char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

						in_len = sizeof in_addr;
						infd = accept(listener, &in_addr, &in_len);
						if (infd == -1) {
							if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
								// We have processed all incoming connections.
								break;
							} else {
								perror("accept");
								break;
							}
						}

						s = getnameinfo(&in_addr, in_len, hbuf, sizeof hbuf,
								sbuf, sizeof sbuf,
								NI_NUMERICHOST | NI_NUMERICSERV);
						if (s == 0) {
							/*	printf("Accepted connection on descriptor %d "
							 "(host=%s, port=%s)\n", infd, hbuf, sbuf);	*/
						}

						// Make the incoming socket non-blocking and add it to the list of fds to monitor.
						s = make_socket_non_blocking(infd);

						reuseSock(infd);

						if (s == -1)
							abort();

						event.data.fd = infd;
						event.events = EPOLLIN | EPOLLET;
						s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
						if (s == -1) {
							perror("epoll_ctl");
							abort();
						}
					} //end while
					continue;

				} //end if(TCP==true)
				else if (TCP == false) {

					sockaddr_in fromAddr;
					char recvBuff[Env::MAX_MSG_SIZE];
					int recvSize = udpRecvFrom(events[i].data.fd, recvBuff,
							sizeof(recvBuff), fromAddr, 0);

					DataEvent data(events[i].data.fd, recvBuff,
							sizeof(recvBuff), fromAddr);
					dataQueue.push(data);
					//dataService(events[i].data.fd, recvBuff, fromAddr, pmap);
					memset(recvBuff, '\0', sizeof(recvBuff));
				}
			} else {

				if (TCP == true) {

					// TCP data on existing connection
					// We have data on the fd waiting to be read. Read and display it. We must read whatever data is available
					//completely, as we are running in edge-triggered mode and won't get a notification again for the same data.
					int done = 0;

					while (1) {

						ssize_t count;
						int sfd = events[i].data.fd;

						char buf[Env::BUF_SIZE];
						memset(buf, '\0', sizeof(buf));

						count = generalReveiveTCP(sfd, buf, Env::BUF_SIZE, 0);

						if (count == -1) {

							// If errno == EAGAIN, that means we have read all data. So go back to the main loop.
							if (errno != EAGAIN) {

								perror("read");
								done = 1;
							}

							break;

						} else if (count == 0) {

							// End of file. The remote has closed the connection.
							done = 1;
							break;

						} else { //count > 0

							bool ready = false;
							string bd = pbrb->getBdStr(sfd, buf, count, ready);

							if (ready) {

								sockaddr_in fromAddr; // no use for TCP, just to fill the parameter
								fromAddr.sin_port = 0;
								fromAddr.sin_addr.s_addr = 0;

								DataEvent data(sfd, (char*) bd.c_str(),
										bd.size(), fromAddr);
//								DataEvent data(events[i].data.fd, buf, fromAddr);
								dataQueue.push(data);
							}

							/*sockaddr_in fromAddr; // no use for TCP, just to fill the parameter
							 fromAddr.sin_port = 0;
							 fromAddr.sin_addr.s_addr = 0;

							 DataEvent data(events[i].data.fd, buf, sizeof(buf),
							 fromAddr);
							 dataQueue.push(data);*/
							//dataService(events[i].data.fd, buf, fromAddr, pmap);//
						}

						memset(buf, '\0', sizeof(buf));
					}

					if (done) {

						// Closing the descriptor will make epoll remove it from the set of descriptors which are monitored.
						close(events[i].data.fd);
						/*
						 printf("Closed connection on descriptor %d\n",
						 events[i].data.fd);*/
					}

				} //if TCP == true

			} //end else
		} //end for
	} //end main while

	free(events);

	close(listener);

	delete pbrb;
	pbrb = NULL;

	return EXIT_SUCCESS;
}

