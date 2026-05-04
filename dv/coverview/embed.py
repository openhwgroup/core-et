#!/bin/env python3

index_filename = "dist/index.html"
index_backup_filename = "dist/index.html.bak"
# Add more files + arguments for choosing names when this all works

import argparse
import base64
import json
import os
from pathlib import Path
import shutil
import re

parser = argparse.ArgumentParser()
parser.add_argument('--inject-data')
parser.add_argument('--fetch-data')
parser.add_argument('--template-url')
args = parser.parse_args()

if (args.inject_data and args.fetch_data):
    print("Cannot inject data and fetch it in runtime at the same time, the flags are contradictory.")
    import sys
    sys.exit(1)

original_snippet = 'let originalFiles = {}'

if os.path.exists(index_backup_filename):
    shutil.copy(index_backup_filename, index_filename)
else:
    shutil.copy(index_filename, index_backup_filename)

with open(index_filename, 'r+') as f:
    content = f.read()

    if args.inject_data:
        if os.path.isfile(args.inject_data) and args.inject_data.endswith('.zip'):
            content = content.replace(original_snippet, 'let originalFiles = {}; let embeddedZipArchive = "' +
                                      base64.b64encode(Path(args.inject_data).read_bytes()).decode('ascii') + '"')
        else:
            files = {}

            cwd = os.getcwd()
            os.chdir(args.inject_data)

            p = Path()
            for filename in list(p.glob("*.info")) + list(p.glob("*.desc")) + ['logo.svg', 'config.json', 'sources.txt']:
                if os.path.isfile(filename):
                    files[str(filename)] = open(str(filename), 'r').read()

            os.chdir(cwd)

            content = content.replace(original_snippet, 'let originalFiles = ' + json.dumps(files))

    elif args.fetch_data:
        fetch_snippet = 'let originalFiles = {}; let fetchData = "%s";' % args.fetch_data
        if args.template_url: fetch_snippet += 'let templatedFetchUrl = "%s";' % args.template_url
        content = content.replace(original_snippet, fetch_snippet)

    r = r'(<script type="module" crossorigin src="/(assets/index-[\w\-]+.js)"></script>)'
    matches = re.findall(r, content)
    for m in matches:
        with open('dist/' + m[1], 'r') as sf:
            scriptText = sf.read()
            content = content.replace(m[0], '<script type="module">' + scriptText + '</script>')
    f.seek(0)
    f.write(content)
    f.truncate()
