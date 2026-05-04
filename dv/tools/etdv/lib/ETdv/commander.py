# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import sys, subprocess, threading, os, time, signal

"""
Run subcommand and process stdout/stderr realtime.
Use subprocess.run for single commands.
"""


class PipedLineReader(threading.Thread):
    """
    Encapsulate pipe reader in a thread.
    """

    def __init__(self, reader):
        """
        Create new line reader thread.
        :param reader: lamdba called for every line (read) from pipe.
        """
        threading.Thread.__init__(self)
        self._reader = reader
        self._fd_read, self._fd_write = os.pipe()
        self._pipe_reader = os.fdopen(self._fd_read)
        self.start()

    def run(self):
        """
        Run the thread, read lines and pass to reader.
        """
        for line in iter(self._pipe_reader.readline, ''):
            self._reader(line.strip('\n'))
        # todo: get exception (randomly) if we do .close()
        if False:
            self._pipe_reader.close()

    def close(self):
        """
        Close the write end of the pipe.
        """
        os.close(self._fd_write)

    def fileno(self):
        """
        Return the write file descriptor of the pipe.
        """
        return self._fd_write

    def stop(self):
        self.close()

    def __del__(self):
        try:
            self.stop()
        except:
            pass
        try:
            del self._fd_read
            del self._fd_write
        except:
            pass


class Commander:
    """
    Object to run commands (with no stdin).
    """

    def __init__(self, command, stdout=None, stderr=None):
        """
        Create instance to run command (with no stdin).
        :param command: command to run.
        :param stdout: lambda to process stdout everytime new line written.
        :param stderr: lambda to process stderr everytime new line written.
        """
        if isinstance(command, str): command = command.split()
        self._command = command
        self._process = None
        self._readers = []
        if stdout is None:
            stdout = lambda line: print(line, flush=True)
        self._stdout = stdout
        if stderr is None:
            stderr = lambda line: print(line, file=sys.stderr, flush=True)
        self._stderr = stderr

    def send_signal(self, signal=signal.SIGINT):
        if self._process is None: return
        self._process.send_signal(signal)
        self._cleanup()

    def _cleanup(self):
        [rdr.close() for rdr in self._readers]

    def run(self, wait=0.1):
        """
        Run command, waiting before we start.
        :param wait: important to wait >= 0.1s or we'll get random pipe-related errors.
        :return: exit code from command.
        """
        assert wait >= 0.1
        time.sleep(wait)
        stdout_reader = PipedLineReader(self._stdout)
        stderr_reader = PipedLineReader(self._stderr)
        self._readers.append(stdout_reader)
        self._readers.append(stderr_reader)
        self._process = subprocess.Popen(self._command,
                                         stdout=stdout_reader,
                                         stderr=stderr_reader,
                                         universal_newlines=True)
        returncode = self._process.wait()
        self._cleanup()
        return returncode


if __name__ == '__main__':
    def runme(cmd):
        print('Command=' + cmd)
        ecode = Commander(cmd.split()).run()
        print('ecode=' + str(ecode))


    runme('ssh jenkins@jenkins tail /home/eng/jenkins/runs/kpfalzer/jenkins-Maxion_Verif-2/all_steps.log')
    runme('cp foo bar')
    runme('git status')
