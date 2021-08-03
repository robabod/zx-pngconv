// pngconv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pngconv.h"
#include "CommandLinePngConv.h"

#include "PngImage.h"
#include "RGBAImage.h"
#include "ZxImage.h"

#include "exceptions.h"

#include <fstream>
#include <ostream>

int main(int argc, char* argv[])
{
	int nRetCode = 0;

	CCommandLinePngConv cmdLine;
	int error = cmdLine.ParseCommandLine(argc, argv);
	if (!cmdLine.m_bSilent || cmdLine.m_bVersion)
	{
		// output some header/credit information
		std::cout << PNGCONV_SHORT_TITLE << " : " << PNGCONV_LONG_TITLE << " v" << PNGCONV_VERSION_STRING << PNGCONV_RELEASE << std::endl;
		std::cout << PNGCONV_COPYRIGHT << std::endl << std::endl;
	}

	if (cmdLine.m_bVersion)
	{
		std::cout << "Built on " << PNGCONV_BUILD_DATE << " @ " << PNGCONV_BUILD_TIME << std::endl << std::endl;
		std::cout << "Includes the following libraries:" << std::endl;
		std::cout << "  libpng v" << PNG_LIBPNG_VER_STRING << std::endl;
		std::cout << "  zlib v" << ZLIB_VERSION << std::endl;
	}
	else if (cmdLine.m_bDisplayHelp || 1 == argc || cmdLine.m_sPngFileName.empty())
	{
		std::cout << cmdLine.Help() << std::endl;
	}
	else if (error)
	{
		std::cerr << "Error with parameter " << error << ": " << argv[error] << std::endl;
		nRetCode = 1;
	}
	else
	{
		// here we'll grab all of our values and create the object that'll do the conversion.
		try
		{
			CPngImage pngimage(cmdLine.m_sPngFileName);

			// if we get here we should have the file open, so read in the image
			pngimage.ReadImage();

			// get the projected image size and create a temporary RGBA image...
			POINT pt = cmdLine.m_pSize;
			if (pt.x < 1 || pt.y < 1)
			{
				pt = pngimage.GetImageSize();
			}

			CRGBAImage rgbaimg(pt);

			// now pass in the CPngImage so we can extract the data we want...
			rgbaimg.ExtractImageData(pngimage, cmdLine.m_pOrigin);

			// we've done that, now we can free up the pngimage
			pngimage.Close();

			// now we'll create a ZX Image in the appropriate format
			CZxImage zximg(pt);

			// set up our conversion options
			zximg.InvertMask(cmdLine.m_bInverseMask);
			zximg.InvertByte(cmdLine.m_bInverseByte);
			zximg.SetMaskFormat(cmdLine.m_nMaskFormat);

			if (cmdLine.m_bUseRGBMask)
				zximg.SetRGBMask(cmdLine.m_rgbaMaskColour);

			zximg.ProcessRGBAImage(rgbaimg);

			// set the output format
			zximg.SetTextOutput(cmdLine.m_bTxtOut);
			zximg.SetReverseOutput(cmdLine.m_bReverse);
			zximg.SetUpsideDown(cmdLine.m_bUpsideDown);
			zximg.SetZigZag(cmdLine.m_bZigZag);
			zximg.SetLeadText(cmdLine.m_sLeadText);
			zximg.SetHexOutput(cmdLine.m_bHexOutput, cmdLine.m_sLeadTextByte);

			// now we'll set up our output stream.
			// we'll send to either stdout or a file.
			// set to stdout here by default, and use a file if need be.
			std::ofstream fileout;
			std::ostream  out(std::cout.rdbuf());

			if (!cmdLine.m_bStdOut)
			{
				// validate the filename
				if (cmdLine.m_sOutFileName.empty() || (cmdLine.m_sOutFileName == cmdLine.m_sPngFileName))
				{
					cmdLine.m_sOutFileName = cmdLine.m_sPngFileName + ".out";
				}

				// open the file
				int open_flags = std::ios::out;
				if (!cmdLine.m_bTxtOut)
					open_flags |= std::ios::binary;
				if (cmdLine.m_bOutputAppend)
					open_flags |= std::ios::app;
				fileout.open(cmdLine.m_sOutFileName.c_str(), open_flags);
				if (!fileout.is_open())
				{
					throw CFileException(cmdLine.m_sOutFileName);
				}
				else
				{
					out.rdbuf(fileout.rdbuf());
				}
			}

			// here we'll pass in the ostream so that
			// we can actually output the data. w00t.
			out << zximg;

			if (fileout.is_open())
				fileout.close();
		}
		catch (CFileException &e)
		{
			std::cerr << "Error accessing file \"" << e.what() << "\""  << std::endl;
		}
		catch (CPngImage::CPngFileException &e)
		{
			std::cerr << "Error with input file: " << e.what() << std::endl;
		}
		catch (CPngImage::CPngLibException &e)
		{
			std::cerr << "Error in pnglib: " << e.what() << std::endl;
		}
		catch (CRGBAImage::CExBadSize &e)
		{
			std::cerr << "Error in output image dimensions: " << e.what() << std::endl;
		}
		catch (CZxImage::CExUnknownMask &e)
		{
			std::cerr << "Error - unknown mask: " << e.what() << std::endl;
		}
		catch (std::bad_alloc &e)
		{
			std::cerr << "Error allocating image memory: " << e.what() << std::endl;
		}
	}

	return nRetCode;
}


