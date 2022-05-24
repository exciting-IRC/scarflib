#!/usr/bin/env python3
"""
create.py: create boilerplate for your c++ project

usage: create.py [-h] [options] <path>

options:
    -h, --help               show this help message and exit
    -n <N>, --namespace <N>  namespace to use
    -p <P>, --parent <P>     root of directory path [default: .]
"""
from __future__ import annotations

from pathlib import Path
from sys import argv

from docopt import docopt

template = """\
{include}

{namespace}
"""


def wrap_with(body: str, wrap: tuple[str, str], *, distance: int = 1) -> str:
    nl = distance * "\n"
    return f"{wrap[0]}{nl}{body}{nl}{wrap[1]}"


def wrap_header(body: str, name: str, suffix: str) -> str:
    ext = suffix.replace(".", "_")
    guard = f"{name}{ext}".upper()
    header = (
        f"#ifndef {guard}\n" f"#define {guard}",
        f"#endif // {guard}",
    )
    return wrap_with(body, header, distance=2)


def base_category_name(stem: str) -> str:
    return stem.rsplit("_", maxsplit=1)[0]


def get_include(path: Path) -> str:
    if path.suffix in [".tpp", ".cpp"]:
        return f'#include "{base_category_name(path.stem)}.hpp"'
    return ""


def get_nested_namespace(names: tuple[str]) -> str:
    begin = "namespace {ns} {{"
    end = "}}  // namespace {ns}"

    begins = "\n".join([begin.format(ns=n) for n in names])
    ends = "\n".join([end.format(ns=n) for n in names])

    return f"{begins}\n\n{ends}"


def main():
    assert __doc__ is not None
    args: dict[str, str] = docopt(__doc__)

    # print(args)

    path = Path(args["<path>"])
    name = f"{path.parent.stem}_{path.stem}"
    include = get_include(path)
    fullpath = Path(__file__).parent / args["--parent"] / args["<path>"]

    namespace = get_nested_namespace(
        (args["--namespace"],)
        if args["--namespace"]
        else fullpath.parent.parts[:1]
    )

    text = template.format(namespace=namespace, include=include)
    if path.suffix != ".cpp":
        text = wrap_header(text, name, path.suffix)

    fullpath.parent.mkdir(parents=True, exist_ok=True)
    if not fullpath.exists():
        fullpath.write_text(text)
        print(text)
    else:
        print("file already exists")


if __name__ == "__main__":
    main()
