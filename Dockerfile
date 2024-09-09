ARG BASE_IMAGE=debian:bullseye-20221219-slim


FROM mcr.microsoft.com/vscode/devcontainers/cpp:bullseye AS dev_container
COPY requirements ./requirements
RUN chmod -R +x ./requirements
RUN ./requirements/install_cmake_3_21_7.sh
RUN ./requirements/install_build_requirements.sh
RUN ./requirements/install_test_requirements.sh
RUN ./requirements/install_clangd_language_server_16_0_2.sh


FROM $BASE_IMAGE AS build
WORKDIR /vca-drivers
COPY requirements ./requirements
RUN chmod -R +x ./requirements
RUN ./requirements/install_cmake_3_21_7.sh
RUN ./requirements/install_build_requirements.sh

COPY drivers ./drivers
COPY template ./template
COPY sdk ./sdk
COPY CMakeLists.txt ./CMakeLists.txt
COPY CMakePresets.json ./CMakePresets.json
COPY build_drivers.sh ./build_drivers.sh


FROM build AS test
RUN ./requirements/install_test_requirements.sh