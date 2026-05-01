import os
import subprocess
from pathlib import Path

import parametrize_from_file as pff


@pff.parametrize
def test_0003(wptt):

    if os.name == "nt":
        exepath = Path.cwd() / "i2pp_cli.exe"
    else:
        exepath = Path.cwd() / "i2pp_cli"

    assert exepath.is_file()

    args = [str(exepath)]
    proc = subprocess.run(
        args,
        input=wptt,
        text=True,
        timeout=10,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    assert proc.returncode != 0, f"{wptt}"
