/* Copyright (c) 2016, 2022, Oracle and/or its affiliates.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have included with MySQL.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

/*
  This operates on the error messages that will be included into the
  client library (libmysql), NOT the messages that the server sends
  to clients!
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <fstream>

using namespace std;

static const char *INFILE = "errmsg.h";
static const char *OUTFILE = "mysqlclient_ername.h";


static void usage() {
  printf(
      "Usage: ./comp_client_err --in_file=name --out_file=name\n"
      "Options: \n"
      " -F, --in_file=name  Input file name\n"
      " -O, --out_file=name Output file name\n");
}


int main(int argc, char *argv[]) {
  const char *error_file = R"MYSQLERRFILE(/* Copyright (c) 2016, 2023, Oracle and/or its affiliates.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have included with MySQL.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */
/* Autogenerated file, please don't edit */

{ "CR_UNKNOWN_ERROR", 2000, "Unknown MySQL error", 0, 0, 0},
{ "CR_SOCKET_CREATE_ERROR", 2001, "Can't create UNIX socket (%d)", 0, 0, 0},
{ "CR_CONNECTION_ERROR", 2002, "Can't connect to local MySQL server through socket '%-.100s' (%d)", 0, 0, 0},
{ "CR_CONN_HOST_ERROR", 2003, "Can't connect to MySQL server on '%-.100s:%u' (%d)", 0, 0, 0},
{ "CR_IPSOCK_ERROR", 2004, "Can't create TCP/IP socket (%d)", 0, 0, 0},
{ "CR_UNKNOWN_HOST", 2005, "Unknown MySQL server host '%-.100s' (%d)", 0, 0, 0},
{ "CR_SERVER_GONE_ERROR", 2006, "MySQL server has gone away", 0, 0, 0},
{ "CR_VERSION_ERROR", 2007, "Protocol mismatch; server version = %d, client version = %d", 0, 0, 0},
{ "CR_OUT_OF_MEMORY", 2008, "MySQL client ran out of memory", 0, 0, 0},
{ "CR_WRONG_HOST_INFO", 2009, "Wrong host info", 0, 0, 0},
{ "CR_LOCALHOST_CONNECTION", 2010, "Localhost via UNIX socket", 0, 0, 0},
{ "CR_TCP_CONNECTION", 2011, "%-.100s via TCP/IP", 0, 0, 0},
{ "CR_SERVER_HANDSHAKE_ERR", 2012, "Error in server handshake", 0, 0, 0},
{ "CR_SERVER_LOST", 2013, "Lost connection to MySQL server during query", 0, 0, 0},
{ "CR_COMMANDS_OUT_OF_SYNC", 2014, "Commands out of sync; you can't run this command now", 0, 0, 0},
{ "CR_NAMEDPIPE_CONNECTION", 2015, "Named pipe: %-.32s", 0, 0, 0},
{ "CR_NAMEDPIPEWAIT_ERROR", 2016, "Can't wait for named pipe to host: %-.64s  pipe: %-.32s (%lu)", 0, 0, 0},
{ "CR_NAMEDPIPEOPEN_ERROR", 2017, "Can't open named pipe to host: %-.64s  pipe: %-.32s (%lu)", 0, 0, 0},
{ "CR_NAMEDPIPESETSTATE_ERROR", 2018, "Can't set state of named pipe to host: %-.64s  pipe: %-.32s (%lu)", 0, 0, 0},
{ "CR_CANT_READ_CHARSET", 2019, "Can't initialize character set %-.32s (path: %-.100s)", 0, 0, 0},
{ "CR_NET_PACKET_TOO_LARGE", 2020, "Got packet bigger than 'max_allowed_packet' bytes", 0, 0, 0},
{ "CR_EMBEDDED_CONNECTION", 2021, "Embedded server", 0, 0, 0},
{ "CR_PROBE_SLAVE_STATUS", 2022, "Error on SHOW SLAVE STATUS:", 0, 0, 0},
{ "CR_PROBE_SLAVE_HOSTS", 2023, "Error on SHOW SLAVE HOSTS:", 0, 0, 0},
{ "CR_PROBE_SLAVE_CONNECT", 2024, "Error connecting to slave:", 0, 0, 0},
{ "CR_PROBE_MASTER_CONNECT", 2025, "Error connecting to master:", 0, 0, 0},
{ "CR_SSL_CONNECTION_ERROR", 2026, "SSL connection error: %-.100s", 0, 0, 0},
{ "CR_MALFORMED_PACKET", 2027, "Malformed packet", 0, 0, 0},
{ "CR_WRONG_LICENSE", 2028, "This client library is licensed only for use with MySQL servers having '%s' license", 0, 0, 0},
{ "CR_NULL_POINTER", 2029, "Invalid use of null pointer", 0, 0, 0},
{ "CR_NO_PREPARE_STMT", 2030, "Statement not prepared", 0, 0, 0},
{ "CR_PARAMS_NOT_BOUND", 2031, "No data supplied for parameters in prepared statement", 0, 0, 0},
{ "CR_DATA_TRUNCATED", 2032, "Data truncated", 0, 0, 0},
{ "CR_NO_PARAMETERS_EXISTS", 2033, "No parameters exist in the statement", 0, 0, 0},
{ "CR_INVALID_PARAMETER_NO", 2034, "Invalid parameter number", 0, 0, 0},
{ "CR_INVALID_BUFFER_USE", 2035, "Can't send long data for non-string/non-binary data types (parameter: %d)", 0, 0, 0},
{ "CR_UNSUPPORTED_PARAM_TYPE", 2036, "Using unsupported buffer type: %d  (parameter: %d)", 0, 0, 0},
{ "CR_SHARED_MEMORY_CONNECTION", 2037, "Shared memory: %-.100s", 0, 0, 0},
{ "CR_SHARED_MEMORY_CONNECT_REQUEST_ERROR", 2038, "Can't open shared memory; client could not create request event (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_CONNECT_ANSWER_ERROR", 2039, "Can't open shared memory; no answer event received from server (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_CONNECT_FILE_MAP_ERROR", 2040, "Can't open shared memory; server could not allocate file mapping (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_CONNECT_MAP_ERROR", 2041, "Can't open shared memory; server could not get pointer to file mapping (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_FILE_MAP_ERROR", 2042, "Can't open shared memory; client could not allocate file mapping (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_MAP_ERROR", 2043, "Can't open shared memory; client could not get pointer to file mapping (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_EVENT_ERROR", 2044, "Can't open shared memory; client could not create %s event (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_CONNECT_ABANDONED_ERROR", 2045, "Can't open shared memory; no answer from server (%lu)", 0, 0, 0},
{ "CR_SHARED_MEMORY_CONNECT_SET_ERROR", 2046, "Can't open shared memory; cannot send request event to server (%lu)", 0, 0, 0},
{ "CR_CONN_UNKNOW_PROTOCOL", 2047, "Wrong or unknown protocol", 0, 0, 0},
{ "CR_INVALID_CONN_HANDLE", 2048, "Invalid connection handle", 0, 0, 0},
{ "CR_UNUSED_1", 2049, "Connection using old (pre-4.1.1) authentication protocol refused (client option 'secure_auth' enabled)", 0, 0, 0},
{ "CR_FETCH_CANCELED", 2050, "Row retrieval was canceled by mysql_stmt_close() call", 0, 0, 0},
{ "CR_NO_DATA", 2051, "Attempt to read column without prior row fetch", 0, 0, 0},
{ "CR_NO_STMT_METADATA", 2052, "Prepared statement contains no metadata", 0, 0, 0},
{ "CR_NO_RESULT_SET", 2053, "Attempt to read a row while there is no result set associated with the statement", 0, 0, 0},
{ "CR_NOT_IMPLEMENTED", 2054, "This feature is not implemented yet", 0, 0, 0},
{ "CR_SERVER_LOST_EXTENDED", 2055, "Lost connection to MySQL server at '%s', system error: %d", 0, 0, 0},
{ "CR_STMT_CLOSED", 2056, "Statement closed indirectly because of a preceding %s() call", 0, 0, 0},
{ "CR_NEW_STMT_METADATA", 2057, "The number of columns in the result set differs from the number of bound buffers. You must reset the statement, rebind the result set columns, and execute the statement again", 0, 0, 0},
{ "CR_ALREADY_CONNECTED", 2058, "This handle is already connected. Use a separate handle for each connection.", 0, 0, 0},
{ "CR_AUTH_PLUGIN_CANNOT_LOAD", 2059, "Authentication plugin '%s' cannot be loaded: %s", 0, 0, 0},
{ "CR_DUPLICATE_CONNECTION_ATTR", 2060, "There is an attribute with the same name already", 0, 0, 0},
{ "CR_AUTH_PLUGIN_ERR", 2061, "Authentication plugin '%s' reported error: %s", 0, 0, 0},
{ "CR_INSECURE_API_ERR", 2062, "Insecure API function call: '%s' Use instead: '%s'", 0, 0, 0},
{ "CR_FILE_NAME_TOO_LONG", 2063, "File name is too long", 0, 0, 0},
{ "CR_SSL_FIPS_MODE_ERR", 2064, "Set FIPS mode ON/STRICT failed", 0, 0, 0},
{ "CR_DEPRECATED_COMPRESSION_NOT_SUPPORTED", 2065, "Compression protocol not supported with asynchronous protocol", 0, 0, 0},
{ "CR_COMPRESSION_WRONGLY_CONFIGURED", 2066, "Connection failed due to wrongly configured compression algorithm", 0, 0, 0},
{ "CR_KERBEROS_USER_NOT_FOUND", 2067, "SSO user not found, Please perform SSO authentication using kerberos.", 0, 0, 0},
{ "CR_LOAD_DATA_LOCAL_INFILE_REJECTED", 2068, "LOAD DATA LOCAL INFILE file request rejected due to restrictions on access.", 0, 0, 0},
{ "CR_LOAD_DATA_LOCAL_INFILE_REALPATH_FAIL", 2069, "Determining the real path for '%s' failed with error (%d): %s", 0, 0, 0},
{ "CR_DNS_SRV_LOOKUP_FAILED", 2070, "DNS SRV lookup failed with error : %d", 0, 0, 0},
{ "CR_MANDATORY_TRACKER_NOT_FOUND", 2071, "Client does not recognise tracker type %d marked as mandatory by server.", 0, 0, 0},
{ "CR_INVALID_FACTOR_NO", 2072, "Invalid first argument for MYSQL_OPT_USER_PASSWORD option. Valid value should be between 1 and 3 inclusive.", 0, 0, 0},
{ "CR_CANT_GET_SESSION_DATA", 2073, "Can't get session data: %s", 0, 0, 0},
)MYSQLERRFILE";
  std::string out_file(OUTFILE);
  for(size_t i = 0; i < argc; i++) {
    std::string arg(argv[i]);
    if (arg.rfind("--out_file=") == 0) {
      out_file = arg.substr(11);
    } else if (arg == "-O" && i + 1 < argc) {
      out_file = argv[i + 1];
    }
  }
  ofstream ofs(out_file, std::ofstream::out);
  ofs << error_file;
  ofs.close();
  return 0;
}
