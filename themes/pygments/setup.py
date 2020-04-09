from setuptools import setup, find_packages
 
setup (
    name='pygments-style-mcodemonokai',
    version='0.0.1',
    author='Jurgen Keller',
    description='Pygments version of the MCODE Monokai theme.',
    packages=find_packages(),
    entry_points=
    """
    [pygments.styles]
    mcodemonokai = mcodemonokai.mcodeMonokai:McodeMonokaiStyle
    """,
)
