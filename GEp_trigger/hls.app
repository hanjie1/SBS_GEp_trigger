<project xmlns="com.autoesl.autopilot.project" name="GEp_trigger" top="hcal_cluster_hls" projectType="C/C++">
    <includePaths/>
    <libraryPaths/>
    <Simulation>
        <SimFlow name="csim" profile="true" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <files xmlns="">
        <file name="hcal_cluster_hls.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="../../hcal_cluster_hls_tb.cpp" sc="0" tb="1" cflags="-lpthread -Wno-unknown-pragmas" csimflags="" blackbox="false"/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

