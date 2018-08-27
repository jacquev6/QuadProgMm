# coding: utf8

# Copyright 2013-2018 Vincent Jacques <vincent@vincent-jacques.net>

import os
import sys
import glob


project = "Quad&shy;Prog&shy;Mm"
author = '<a href="http://vincent-jacques.net/">Vincent Jacques</a>'
copyright = ('2010-2018 {} <script>var jacquev6_ribbon_github="{}"</script>'.format(author, project) +
             '<script src="https://jacquev6.github.io/ribbon.js"></script>')


master_doc = "index"
extensions = []

# https://github.com/bitprophet/alabaster
html_sidebars = {
    "**": ["about.html", "navigation.html", "searchbox.html"],
}
html_theme_options = {
    "github_user": "jacquev6",
    "github_repo": project,
    "travis_button": False,
}


# http://sphinx-doc.org/ext/githubpages.html
extensions.append("sphinx.ext.githubpages")


# http://sphinx-doc.org/latest/ext/math.html
extensions.append("sphinx.ext.mathjax")
# mathjax_path


for input_file in ["../README.rst"] + glob.glob("user_guide/*.rst"):
    with open(input_file) as in_f:
        seen = set()
        out_f = None
        output_file = None
        for line in in_f:
            if line.rstrip() == ".. END SECTION {}".format(output_file):
                assert output_file is not None
                out_f.close()
                out_f = None
                output_file = None
            if out_f:
                if any(line.startswith(prefix) for prefix in ["    ", "... ", ">>> "]):
                    out_f.write(line[4:])
                elif line.strip() == "":
                    out_f.write("\n")
            if line.startswith(".. BEGIN SECTION "):
                assert output_file is None
                output_file = line[17:].rstrip()
                if output_file in seen:
                    mode = "a"
                else:
                    mode = "w"
                seen.add(output_file)
                out_f = open("user_guide/artifacts/{}".format(output_file), mode)
        assert output_file is None
