# About #

General Plugin Template Project (aka GPTP) is a C++ project for quickly building MPQDraft plugins for StarCraft: Brood War. GPTP was originally developed by A\_of\_s\_t, but was abandoned for quite a long time until the folks at BroodWarAi.com eventually picked it up and started coding again.


# News #

## GPTP v2.41 Released ##
[Download link is available here](https://sites.google.com/site/sctransition/download/GPTP-v2.41.zip?attredirects=0&d=1).

Changes:
  * New hook module for controlling attack priority
  * Basic logging mechanism for writing to text files
  * Many new API functions
  * Several bug fixes

Check out the [full changelog](Changelog.md) for details.

_Note: The wiki is out of date at the moment. We will update the documentation during the next several days. In the meantime, please see the header files (e.g. `SCBW/structures/CUnit.h` and `SCBW/api.h`) for function and class information._

## GPTP v2.4 Released ##
[Download it now!](https://sites.google.com/site/sctransition/download/GPTP-v2.4.zip?attredirects=0&d=1)

This version introduces many big changes:
  * GPTP has moved to Visual C++ 2010 Express, and now uses C++11 features such as lambdas, nullptr, and static\_assert().
  * GPTP now checks the version of StarCraft (1.16.1) that the plugin is loaded in.
  * Hook modules have been categorized.
  * New functions and data variables have been added.
  * Fixed several bugs that prevented GPTP from properly emulating StarCraft.

See the [full changelog](Changelog.md) for more information.

## Downloads Page Moved to Google Sites ##
The Downloads page has been moved to a Google Sites website because some people were reporting that Google Drive did not work on their end.

