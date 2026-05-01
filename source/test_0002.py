import os
import subprocess
from pathlib import Path

import parametrize_from_file as pff


@pff.parametrize
def test_0002(wptt):

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

    assert proc.returncode != 0
