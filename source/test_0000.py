import os
import subprocess
from pathlib import Path

import parametrize_from_file as pff


@pff.parametrize
def test_0000(wptt, pl):

    if os.name == "nt":
        exepath = Path.cwd() / "i2pp_cli.exe"
    else:
        exepath = Path.cwd() / "i2pp_cli"

    assert exepath.is_file()

    args = [str(exepath), "-n", wptt]
    proc = subprocess.run(
        args,
        input=bytes(wptt.encode("utf-8")),
        capture_output=True,
        timeout=10,
    )
    assert proc.returncode == 0

    program_output = proc.stdout.decode("utf-8")

    assert program_output == pl, "Value was incorrect"
