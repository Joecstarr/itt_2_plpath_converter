import os
import subprocess
from pathlib import Path

import parametrize_from_file as pff


@pff.parametrize
def test_0003(wptt):

    exepath = Path.cwd() / "itt2plp.exe" if os.name == "nt" else Path.cwd() / "itt2plp"

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
