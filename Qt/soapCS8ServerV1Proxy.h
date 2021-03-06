/* soapCS8ServerV1Proxy.h
   Generated by gSOAP 2.7.13 from cs8server.h
   Copyright(C) 2000-2009, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapCS8ServerV1Proxy_H
#define soapCS8ServerV1Proxy_H
#include "soapH.h"

class SOAP_CMAC CS8ServerV1Proxy : public soap
{ public:
	/// Endpoint URL of service 'CS8ServerV1Proxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	CS8ServerV1Proxy();
	/// Constructor with copy of another engine state
	CS8ServerV1Proxy(const struct soap&);
	/// Constructor with engine input+output mode control
	CS8ServerV1Proxy(soap_mode iomode);
	/// Constructor with engine input and output mode control
	CS8ServerV1Proxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~CS8ServerV1Proxy();
	/// Initializer used by constructor
	virtual	void CS8ServerV1Proxy_init(soap_mode imode, soap_mode omode);
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Put SOAP Header in message
	virtual	void soap_header(int *ns1__sessionId);
	/// Get SOAP Header structure (NULL when absent)
	virtual	const SOAP_ENV__Header *soap_header();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Force close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
	virtual	void soap_stream_fault(std::ostream&);
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'getApplications' (returns error code or SOAP_OK)
	virtual	int getApplications(_ns2__getApplications *ns2__getApplications, _ns2__getApplicationsResponse *ns2__getApplicationsResponse);

	/// Web service operation 'getApplicationDatas' (returns error code or SOAP_OK)
	virtual	int getApplicationDatas(_ns2__getApplicationDatas *ns2__getApplicationDatas, _ns2__getApplicationDatasResponse *ns2__getApplicationDatasResponse);

	/// Web service operation 'getRecords' (returns error code or SOAP_OK)
	virtual	int getRecords(_ns2__getRecords *ns2__getRecords, _ns2__getRecordsResponse *ns2__getRecordsResponse);

	/// Web service operation 'getRecord' (returns error code or SOAP_OK)
	virtual	int getRecord(_ns2__getRecord *ns2__getRecord, _ns2__getRecordResponse *ns2__getRecordResponse);

	/// Web service operation 'getJointRange' (returns error code or SOAP_OK)
	virtual	int getJointRange(_ns2__getJointRange *ns2__getJointRange, _ns2__getJointRangeResponse *ns2__getJointRangeResponse);
};
#endif
