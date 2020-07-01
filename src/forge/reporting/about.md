# Forge Reports

The reports here are used for issuing warning / info / error reports to the user. Each report will trigger a forge signal to exit if the report level is "*ERROR*". 

***Report*** - This is the base report object that the others inherit from. This shouldn't be used for making your own reports, as the Reporter object won't be able to handle it. Use one of the following:

***Syntax Report*** - Report a syntax error. If the correct information is given (see the header) the line causing the issue will be displayed with an indication of where in the line the error occured

***Semantic Report*** - Similar functionality as the syntax error, but for semantic issues

***Internal Report*** - Always at an "ERROR" level, the internal report is for reporting internal code issues that caused a problem. It allows passing in detailed error reports for debugging.

***Custom Report*** - A simple report that can be set at any level for reporting errors that aren't any of the above three. The intended use is temporary error reporting for use while constructing 
                a program that uses the forge.

# Reporter

An instance of this is contained in the Forge; It is used for issuing reports. It can handle the predefined reports and issue forge signals based on those reports. 

To implement your own error reporting system you can override the signal handler for forge, and implement your own report / report handler and pipe your errors through that. 
