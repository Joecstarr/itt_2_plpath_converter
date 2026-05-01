import os
import subprocess
from pathlib import Path

import parametrize_from_file as pff


@pff.parametrize
def test_0001(wptt, pl):

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

    program_output = proc.stdout

    assert proc.returncode == 0, f"`{wptt}`"

    assert program_output == pl, "Value was incorrect"
