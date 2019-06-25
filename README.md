# CommonIndexFileFormat
Code to read Jimmy Lin's Common Index File Format files without using protobuf

The format is describes by the protocol buffer definition:

	syntax = "proto3";
	package io.anserini.cidxf;
	message Posting {
	  int32 docid = 1;
	  int32 tf = 2;
	}
	message PostingsList {
	  string term = 1;
	  int64 df = 2;
	  int64 cf = 3;
	  repeated Posting posting = 4;
	}

Each postings list is written in the protobuf Delimited format.  This means that you don't need to read the entire file into memory to process it - but this program does.
