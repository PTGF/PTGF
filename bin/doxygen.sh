#!/bin/bash

doxyfile="Doxyfile"

function show_help {
  echo -ne "\nUsage: \n\t$0 [options]\n\n"
  echo -ne "Options: \n\t--source=\n\t--doxyfile= \n\t--install= \n\t--uninstall= \n\t--debug\n\n"
  echo -ne "Example usage: \n\t$0 --install=\${INSTALL_ROOT}/share/ptgf/\n\n"
}

if [ "$#" == "0" ]; then
  show_help
  exit 1
fi

for i in "$@"; do
  case $i in
    -h|--help)
      show_help
      exit 0
    ;;


    -d|--debug)
      debug=true
    ;;


    --build=*)
      build=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
      cd ${build}
    ;;


    --source=*)
      source=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
    ;;


    --install=*)
      if [ -n "${uninstall}" ]; then
        echo -ne "\nERROR: Only one of the following options can be used simultaneously: 'install' & 'uninstall'\n"
        show_help;
        exit 1;
      fi
      install=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
    ;;

    --install)
      if [ -n "${uninstall}" ]; then
        echo -ne "\nERROR: Only one of the following options can be used simultaneously: 'install' & 'uninstall'\n"
        show_help;
        exit 1;
      fi

      if [ -z "${INSTALL_ROOT}" ]; then
        echo -ne "\nERROR: An install path must be specified in the argument line or through the '\$INSTALL_ROOT' variable\n"
        show_help;
        exit 1;
      fi

      install=${INSTALL_ROOT}
    ;;


    --uninstall=*)
      if [ -n "${install}" ]; then
        echo -ne "\nERROR: Only one of the following options can be used simultaneously: 'install' & 'uninstall'\n"
        show_help;
        exit 1;
      fi
      uninstall=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
    ;;

    --uninstall)
      if [ -n "${install}" ]; then
        echo -ne "\nERROR: Only one of the following options can be used simultaneously: 'install' & 'uninstall'\n"
        show_help;
        exit 1;
      fi

      if [ -z "${INSTALL_ROOT}" ]; then
        echo -ne "\nERROR: An uninstall path must be specified in the argument line or through the '\$INSTALL_ROOT' variable\n"
        show_help;
        exit 1;
      fi

      uninstall=${INSTALL_ROOT}
    ;;


    --doxyfile=*)
      doxyfile=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
    ;;
    *)
      show_help
      exit 1
    ;;
  esac
done


html="doc/html"
compiledHelp="${install}/html/PTGF.qch"
#images="Media/Images"
#logo="Logo_320.png"


# Perform an uninstallation
if [ -n "${uninstall}" ]; then
  echo -ne "\nUninstalling documentation from '${uninstall}'..."
  rm -rf ${uninstall}/${html}
  echo -ne " done.\n\n"
  exit 0
fi

# Check that the doxyfile exists
if [ -f "${doxyfile}" ]; then
echo "sed -r 's|^\s*INPUT\s*=.*\$|INPUT = ${source}|g' ${doxyfile} > ${PWD}/Doxyfile"

  sed -r "s|^\s*INPUT\s*=.*\$|INPUT = ${source}|g" ${doxyfile} > ${PWD}/Doxyfile
else
  echo -ne "\nERROR: Please ensure that you have specified a doxyfile that actually exists: \n\t'${doxyfile}'\n"
  show_help
  exit 1
fi

#Generate the documentation
echo -ne "\nRunning doxygen. Please wait... "

if [ -n "${debug}" ]; then
  echo
  doxygen ${PWD}/Doxyfile
else
  doxygen ${PWD}/Doxyfile > "/dev/null" 2> "/dev/null"
fi

if [ ${?} -ne "0" ]; then
  echo -ne "ERROR: Doxygen returned errors\n"
  exit ${?}
else
  echo -ne "[ OK ]\n"
fi

  #Copy the logo image into the documentation directory
#  echo -ne "  Copying logo image... "
#  if ( [ -f "${images}/${logo}" ] && [ -d "${html}" ] ); then
#    cp "${images}/${logo}" "${html}/"
#        if [ ! -f "${html}/${logo}" ]; then
#          echo -ne "[FAIL]\nLogo image failed to copy\n"
#      exit 1
#    else
#      echo -ne "[ OK ]\n"
#        fi

#  else

#    echo -ne "[FAIL]\nDirectory structure not found and/or file is missing; cannot copy logo image\n"
#    exit 1

#  fi


if [ -n "${install}" ]; then
  mkdir -p ${install}
  cp -R ${html}/ ${install}/

  if [ -f ${compiledHelp} ]; then
    mv ${compiledHelp} ${install}/
  fi
fi


exit 0

