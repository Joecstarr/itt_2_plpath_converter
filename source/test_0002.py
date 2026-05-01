import os
import subprocess
from pathlib import Path

import parametrize_from_file as pff


@pff.parametrize
def test_0002(wptt):

    exepath = Path.cwd() / "itt2plp.exe" if os.name == "nt" else Path.cwd() / "itt2plp"

    assert exepath.is_file()

    args = [str(exepath), "-n", wptt]
    proc = subprocess.run(
        args,
        input=bytes(wptt.encode("utf-8")),
        capture_output=True,
        timeout=10,
    )

    assert proc.returncode != 0
