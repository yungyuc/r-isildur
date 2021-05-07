\NeedsTeXFormat{LaTeX2e}

\ProvidesClass{turgon}[2018/08/13 Formatting for turgon documents]

\LoadClass[11pt,dvips]{report}

% Geometry.
\usepackage{geometry}
\geometry{a4paper,
left=1.2cm,
right=1.2cm,
top=1.5cm,
bottom=1.5cm,
}
% Global functionalities.
\usepackage[authoryear,round,sort]{natbib}
\usepackage{hyperref}
\usepackage{pdfsync}
% encoding.
\usepackage[utf8]{inputenc}
% Mathematics.
\usepackage{amsmath}
\usepackage{amssymb}
\usepackage{amsfonts}
\usepackage{amsthm}
\usepackage{arydshln}
% Authoring.
%\usepackage{authblk}
\usepackage{xcolor}
\usepackage{graphicx}
\graphicspath{{isildur_eps/}}
\usepackage{caption}
\usepackage{subfigure}
\usepackage{paralist}
\usepackage{multirow}
\usepackage{booktabs}
\usepackage{setspace}

\renewcommand{\abstractname}{Forewords}
\renewcommand{\figurename}{Fig.}
\newcommand{\topcaption}{%
\setlength{\abovecaptionskip}{0pt}%
\setlength{\belowcaptionskip}{10pt}%
\caption}
\numberwithin{equation}{chapter}
\captionsetup{width=0.9\textwidth}

% Math commands.
\newcommand{\defeq}{\ensuremath{\buildrel {\text{def}}\over{=}}}
\newcommand{\dif}{\mathop{}\!\mathrm{d}}

\synctex=1

% vim: set ft=tex et sw=2 ts=2 sts=2 tw=79: