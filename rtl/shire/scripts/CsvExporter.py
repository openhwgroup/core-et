#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import apiclient.discovery
import httplib2
import oauth2client
import re
import requests
import shutil
import urllib.parse
import sys
import os
import time

from XsrFileIO import mkdir_p

class CsvExporter:
  SCOPES = 'https://www.googleapis.com/auth/drive.readonly'

  def __init__(self, spreadsheetId):
    store = oauth2client.file.Storage('credentials.json')
    creds = store.get()
    if not creds or creds.invalid:
      flow = oauth2client.client.flow_from_clientsecrets('client_secret.json', CsvExporter.SCOPES)
      creds = oauth2client.tools.run_flow(flow, store)
  
    service = apiclient.discovery.build('sheets', 'v4', http=creds.authorize(httplib2.Http()))
    self.result = service.spreadsheets().get(spreadsheetId = spreadsheetId).execute()
    spreadsheetUrl = self.result['spreadsheetUrl']
    self.exportUrl = re.sub("\/edit$", '/export', spreadsheetUrl)
    self.headers = {
      'Authorization': 'Bearer ' + creds.access_token,
    }

  def export(self, mainSheet, secSheetsDir):
    """ Saves first sheet in 'mainSheet', other sheets in directory secSheetsDir with file name = sheetName.csv
    """

    mkdir_p(secSheetsDir)
    first=True
    for sheet in self.result['sheets']:
      params = {
        'format': 'csv',
        'gid': sheet['properties']['sheetId']
      }

      if first:
        filePath = mainSheet
        first = False
      else:
        filePath = secSheetsDir + sheet['properties']['title'] + '.csv'

      print("Downloading %s" % filePath)
      queryParams = urllib.parse.urlencode(params)
      url = self.exportUrl + '?' + queryParams

      retry_attempts = 8
      backoff = 1
      while True:
        response = requests.get(url, headers = self.headers)
        if response.status_code == 429:
          if retry_attempts > 0:
            retry_attempts-=1
            print (" ... got too many requests => retrying in %ds" % backoff)
            time.sleep(backoff)
            backoff=backoff*2
          else:
            raise Exception("too many requests error after several retries with exp backoff")
        else:
          break

      with open(filePath, 'wb') as csvFile:
        csvFile.write(response.content)
