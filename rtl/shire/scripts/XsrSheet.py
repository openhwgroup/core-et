# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from __future__ import print_function
import sys
import io
from googleapiclient.discovery import build
from httplib2 import Http
from oauth2client import file, client, tools
from collections import defaultdict
from apiclient.http import MediaIoBaseDownload
import csv

class XsrSheet:
    """Connection to google sheets to retrieve data or reading from CSV"""
    SCOPES = ['https://www.googleapis.com/auth/spreadsheets.readonly',
              'https://www.googleapis.com/auth/drive.readonly']
    credentials = 'credentials.json'
    clientSecret = 'client_secret.json'

    instance = None
    gdocsSpreadSheetId = None
    mainSheet = None
    csvBaseDir = None
    
    XLS_MIME_TYPE = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'
    ODS_MIME_TYPE = 'application/x-vnd.oasis.opendocument.spreadsheet'
    CSV_MIME_TYPE = 'text/csv'

    def __init__ (self, conf):
        if conf == None:
            raise Exception("no conf provided")
        
        if conf.csvConf == None:
            self.csv = False
            if conf.gdocsConf.sheetId == None:
                raise Exception("no spreadhshet id provided")
            XsrSheet.gdocsSpreadSheetId = conf.gdocsConf.sheetId
            XsrSheet.mainSheet = conf.gdocsConf.mainSheet
            # Setup the Sheets API
            store = file.Storage(XsrSheet.credentials)
            creds = store.get()
            # open browser to ask for permissions the first time
            if not creds or creds.invalid:
                flow = client.flow_from_clientsecrets(XsrSheet.clientSecret, XsrSheet.SCOPES)
                creds = tools.run_flow(flow, store)
            self.service = build('sheets', 'v4', http=creds.authorize(Http()))
            self.drive_service = build('drive', 'v3', http=creds.authorize(Http()))
            if conf.gdocsConf.download_filename:
                self.download_gdocs(conf.gdocsConf.download_filename, XsrSheet.XLS_MIME_TYPE)
            self.getRevision()
        else:
            self.csv = True
            XsrSheet.mainSheet = conf.csvConf.regsFile
            XsrSheet.csvBaseDir = conf.csvConf.base_dir            

    def download_gdocs(self, filename, mimeType = XLS_MIME_TYPE):
        request = self.drive_service.files().export_media(fileId=XsrSheet.gdocsSpreadSheetId,
                                                          mimeType=mimeType)
        fh = open(filename, "wb") 
        downloader = MediaIoBaseDownload(fh, request)
        done = False
        while done is False:
            status, done = downloader.next_chunk()
        fh.close()

        
    def getRevision(self):
        nextPage=None
        while True:
            revisions = self.drive_service.revisions().list(fileId=XsrSheet.gdocsSpreadSheetId, pageToken=nextPage).execute()
            nextPage = revisions.get('nextPageToken')
            if not nextPage:
                break;
            
        rev = revisions['revisions'][-1]
        with open ("sheet_revision.txt", "w") as fh:
            fh.write( ("%s" % rev ))


    def getMainSheet(self):
        return self.get(XsrSheet.mainSheet)

    def get(self, sheetName):
        if self.csv:
            return self.get_csv(sheetName)
        else:
            return self.get_gdocs(sheetName)
        
    def get_gdocs(self, sheetName):
        # and retrieve the data
        result = self.service.spreadsheets().values().get(spreadsheetId=XsrSheet.gdocsSpreadSheetId, range=sheetName).execute()
        values = result.get('values', [])
        if not values:
            raise Exception ("no data retrieved from google sheets")
        return values

    def get_csv(self, sheetName):
        # compose file name... in subdir types if not the main sheet

        if sheetName == XsrSheet.mainSheet:
            filename =  XsrSheet.csvBaseDir + '/' + sheetName + '.csv'
        else:
            filename =  XsrSheet.csvBaseDir + '/types/' + sheetName + '.csv'

        values = []
        with open(filename, 'r') as csvfile:
            csv_iterator = csv.reader(csvfile)
            for row in csv_iterator:
                # remove empty columns at the end
                while len(row) > 0 and len(row[-1]) == 0:
                    row.pop()
                values.append(row)
                
        # exception expected to be raised if file not found
        return values
    
    @classmethod
    def getInstance(XrsSheet, conf = None):
        if XsrSheet.instance == None:
            XsrSheet.instance = XsrSheet(conf)
        return XsrSheet.instance
