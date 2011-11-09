import re
import os
import itertools
import subprocess

from django.shortcuts import render_to_response, render
from django.template.loader import render_to_string
from django.views.generic import TemplateView
from django.core.urlresolvers import reverse
from django.http import HttpResponse

index = TemplateView.as_view( template_name = "QuadProgMm/index.html" )

class ValidationException( Exception ):
    pass

def generate( request ):
    variables, objectives, constraints = extractInput( request )
    return render_to_response(
        'QuadProgMm/generate.cpp',
        { "variables": variables, "objectives": objectives, "constraints": constraints },
        mimetype = "text/plain"
    )

def compile( request ):
    variables, objectives, constraints = extractInput( request )
    try:
        validateInput( variables, objectives, constraints )
        code = render_to_string(
            'QuadProgMm/generate.cpp',
            { "variables": variables, "objectives": objectives, "constraints": constraints },
        )
        exeFile = os.tmpnam()
        compiler = subprocess.Popen(
            [ 
                "g++",
                "-x", "c++", "-",
                "-I", "/home/vincent/Programming/Projects/QuadProgMm/build/linux_gcc_debug/inc",
                "-L", "/home/vincent/Programming/Projects/QuadProgMm/build/linux_gcc_debug/lib",
                "-l", "qp",
                "-o", exeFile
            ],
            stdin = subprocess.PIPE,
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE
        )
        ( stdoutdata, stderrdata ) = compiler.communicate( code )
        response = HttpResponse( mimetype = 'text/plain' )
        response.write( stdoutdata )
        response.write( stderrdata )
        exe = subprocess.Popen( [ exeFile ], stdout = subprocess.PIPE, stderr = subprocess.PIPE, env = { "LD_LIBRARY_PATH": "/home/vincent/Programming/Projects/QuadProgMm/build/linux_gcc_debug/lib" } )
        ( stdoutdata, stderrdata ) = exe.communicate()
        os.unlink( exeFile )
        response.write( stdoutdata )
        response.write( stderrdata )
        return response
    except ValidationException, e:
        response = HttpResponse( mimetype = 'text/plain' )
        response.write( e )
        return response

def extractInput( request ):
    if "variables" in request.GET:
        variables = request.GET[ "variables" ].split( "\n" )
    else:
        variables = list()

    if "objectives" in request.GET:
        objectives = request.GET[ "objectives" ].split( "\n" )
    else:
        objectives = list()

    if "constraints" in request.GET:
        constraints = request.GET[ "constraints" ].split( "\n" )
    else:
        constraints = list()

    return variables, objectives, constraints


class validateInput:
    def __init__( self, variables, objectives, constraints ):
        self.__variables = variables
        self.__objectives = objectives
        self.__constraints = constraints
        self.__qfRe = "(([-+/*0-9 ()]|" + "|".join( variables ) + ")+)"
        
        exception = ""
        for e in itertools.chain( self.validateVariables(), self.validateObjectives(), self.validateConstraints() ):
            exception += e
        if exception != "":
            raise ValidationException( exception )

    def validateVariables( self ):
        if len( self.__variables ) > 10:
            raise ValidationException( "Too much variables for this demo, I have to pay attention to my CPU usage, sorry...\n" )

        for i, v in enumerate( self.__variables ):
            if not re.match( "^[a-zA-z]\w*$", v ):
                yield "Variable %i/%i '%s' is ill-formed.\n" % ( i + 1, len( self.__variables ), v )

    def validateObjectives( self ):
        endObjectiveRe = "(,[ 0-9.]+)?\)$"
        for i, o in enumerate( self.__objectives ):
            if ( not re.match( "^(Minimize|Maximize)\(" + self.__qfRe + endObjectiveRe, o ) and
                 not re.match( "^Value\(" + self.__qfRe + ",[ 0-9.]+" + endObjectiveRe, o ) and
                 not re.match( "^Difference\(" + self.__qfRe + "," + self.__qfRe + ",[ 0-9.]+" + endObjectiveRe, o ) ):
                yield "Objective %i/%i '%s' is ill-formed.\n" % ( i + 1, len( self.__objectives ), o )

    def validateConstraints( self ):
        for i, c in enumerate( self.__constraints ):
            if not re.match( "^" + self.__qfRe + "(==|>=|<=)" + self.__qfRe + "$", c ):
                yield "Constraint %i/%i '%s' is ill-formed.\n" % ( i + 1, len( self.__constraints ), c )

