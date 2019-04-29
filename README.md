[![Build Status](https://travis-ci.com/pqrs-org/cpp-osx-session.svg?branch=master)](https://travis-ci.com/pqrs-org/cpp-osx-session)
[![License](https://img.shields.io/badge/license-Boost%20Software%20License-blue.svg)](https://github.com/pqrs-org/cpp-osx-session/blob/master/LICENSE.md)

# cpp-osx-session

A utility of `SCDynamicStoreCopyConsoleUser`.

## Requirements

cpp-osx-session depends the following classes.

- [Nod](https://github.com/fr00b0/nod)
- [pqrs::cf::boolean](https://github.com/pqrs-org/cpp-cf-boolean)
- [pqrs::cf::number](https://github.com/pqrs-org/cpp-cf-number)
- [pqrs::cf::string](https://github.com/pqrs-org/cpp-cf-string)
- [pqrs::dispatcher](https://github.com/pqrs-org/cpp-dispatcher)

## Install

### Using package manager

You can install `include/pqrs` by using [cget](https://github.com/pfultz2/cget).

```shell
cget install pqrs-org/cget-recipes
cget install pqrs-org/cpp-osx-session
```

### Manual install

Copy `include/pqrs` directory into your include directory.
