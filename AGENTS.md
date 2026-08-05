# AGENTS.md

## Repository Overview

**Lake-Level** is a Python project for lake level monitoring in East Tennessee. It is currently in early development with no source code committed yet.

## Repository Structure

```
Lake-Level/
├── .gitignore      # Python-focused gitignore
├── LICENSE         # Project license
└── README.md       # Brief project description
```

## Development Environment

This project uses **Python**. Common Python tooling referenced in `.gitignore` includes:
- Virtual environments: `venv`, `.venv`, `env`
- Package managers: `pip`, `pipenv`, `poetry`, `pdm`, `uv`, `pixi`
- Testing: `pytest`
- Type checking: `mypy`, `pytype`, `pyre`
- Linting: `ruff`
- Notebooks: Jupyter, Marimo
- Web frameworks: Django, Flask, Streamlit, Scrapy

## Getting Started

1. Clone the repository.
2. Create and activate a virtual environment:
   ```bash
   python -m venv .venv
   source .venv/bin/activate  # On Windows: .venv\Scripts\activate
   ```
3. Install dependencies once a `requirements.txt` or `pyproject.toml` is present:
   ```bash
   pip install -r requirements.txt
   ```

## Agent Guidelines

- **Language**: Python. Follow PEP 8 style conventions.
- **Linting**: Use `ruff` for linting and formatting if configured.
- **Testing**: Use `pytest` for running tests once a test suite exists.
- **Secrets**: Never commit credentials, API keys, or `.env` files. The `.gitignore` already excludes `.env` and `.envrc`.
- **Dependencies**: Add new packages via `pip install <package>` and record them in a `requirements.txt` or `pyproject.toml`.
- **Scope**: This project monitors lake levels in East Tennessee. Any data sources, APIs, or sensors relevant to that domain are in scope.

## Notes

- No source code exists yet. When adding initial code, establish a clear project layout (e.g., `src/`, `tests/`) and update this file accordingly.
- If a CI workflow is added, document the commands to lint, test, and build here.
